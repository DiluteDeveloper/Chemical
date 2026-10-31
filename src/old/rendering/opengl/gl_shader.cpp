#include "rendering/opengl/gl_shader.h"

#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace Chemical {
  namespace OpenGL {

    uint32_t Shader::CompileShader(const std::string_view& source, int type) {

      unsigned int shader = glCreateShader(type);

      const char* c_source = source.data();
      const GLint c_length = (GLint)source.size();
      glShaderSource(shader, 1, &c_source, &c_length);

      glCompileShader(shader);

      int success = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (success != GL_TRUE) {

        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, info_log);
        SPDLOG_ERROR(R"(Shader "{}" failed to compile : {})", source, info_log);

        glDeleteShader(shader);
        return 0;
      }

      return shader;
    }
    bool Shader::FinaliseProgram() {

      SPDLOG_INFO(R"(Compiling shader program)");

      glLinkProgram(id);

      int success = 0;
      glGetProgramiv(id, GL_LINK_STATUS, &success);
      if (success != GL_TRUE) {
        char info_log[1024];
        glGetProgramInfoLog(id, 1024, nullptr, info_log);

        SPDLOG_ERROR(R"(Shader program failed to compile : {})", info_log);
        return false;
      }
      glValidateProgram(id);

      SPDLOG_INFO(R"(Caching shader program uniform locations)");

      uniform_locations.clear();
      int uniform_count = 0;
      glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_count);

      if (uniform_count > 0) {
        int max_name_len = 0;
        int length = 0;
        int count = 0;
        GLenum type = 0;
        glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i) {
          glGetActiveUniform(id, i, max_name_len, &length, &count, &type, uniform_name.get());

          int location = glGetUniformLocation(id, uniform_name.get());
          uniform_locations[uniform_name.get()] = location;
        }
      }
      return true;
    }

    void Shader::CreateGLShader(const ShaderTraits& traits) {

      // OpenGL ES 3.0 / WebGL2 only has vertex and fragment shaders.
      if (traits.cs_source != std::nullopt || traits.gs_source != std::nullopt) {
        SPDLOG_ERROR("Setting compile_status to -1 : compute and geometry shaders are not supported by "
                     "OpenGL ES 3.0");
        compile_status = -1;
        return;
      }

      uint32_t vs = 1, fs = 1;

      SPDLOG_INFO("Creating OpenGL shader program object");
      id = glCreateProgram();

      if (traits.vs_source != std::nullopt) {
        vs = CompileShader(traits.vs_source.value(), GL_VERTEX_SHADER);
        if (vs != 0) {
          SPDLOG_INFO("Attaching vertex shader to shader program");
          glAttachShader(id, vs);
        }
        glDeleteShader(vs);
      }
      if (traits.fs_source != std::nullopt) {
        fs = CompileShader(traits.fs_source.value(), GL_FRAGMENT_SHADER);
        if (fs != 0) {
          SPDLOG_INFO("Attaching fragment shader to shader program");
          glAttachShader(id, fs);
        }
        glDeleteShader(fs);
      }

      if (vs == 0 || fs == 0) {
        SPDLOG_ERROR("Terminating shader program finalisation and setting compile_status to -1 : one or more "
                     "shaders failed to compile");
        compile_status = -1;
      } else {
        if (!FinaliseProgram()) {
          SPDLOG_ERROR("Setting compile_status to -1 : failed to finalise shader program");
          compile_status = -1;
        }
      }
    }
    void Shader::DeleteGLShader() const {
      glDeleteProgram(id);
    }

    void Shader::Bind() const {
      glUseProgram(id);
    }

    int Shader::Location(const std::string_view& name) const {
      const auto find = uniform_locations.find(std::string(name));
      return find == uniform_locations.end() ? -1 : find->second;
    }

    // All setters below upload to the currently bound program (see Bind()).

    void Shader::SetUniform1F(const std::string_view& name, float value) const {
      glUniform1f(Location(name), value);
    }
    void Shader::SetUniform2F(const std::string_view& name, float value1, float value2) const {
      glUniform2f(Location(name), value1, value2);
    }
    void Shader::SetUniform3F(const std::string_view& name, float value1, float value2, float value3) const {
      glUniform3f(Location(name), value1, value2, value3);
    }
    void Shader::SetUniform4F(const std::string_view& name, float value1, float value2, float value3,
                              float value4) const {
      glUniform4f(Location(name), value1, value2, value3, value4);
    }
    void Shader::SetUniform1I(const std::string_view& name, int value) const {
      glUniform1i(Location(name), value);
    }
    void Shader::SetUniform2I(const std::string_view& name, int value1, int value2) const {
      glUniform2i(Location(name), value1, value2);
    }
    void Shader::SetUniform3I(const std::string_view& name, int value1, int value2, int value3) const {
      glUniform3i(Location(name), value1, value2, value3);
    }
    void Shader::SetUniform4I(const std::string_view& name, int value1, int value2, int value3,
                              int value4) const {
      glUniform4i(Location(name), value1, value2, value3, value4);
    }
    void Shader::SetUniform1UI(const std::string_view& name, unsigned int value) const {
      glUniform1ui(Location(name), value);
    }
    void Shader::SetUniform2UI(const std::string_view& name, unsigned int value1, unsigned int value2) const {
      glUniform2ui(Location(name), value1, value2);
    }
    void Shader::SetUniform3UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                               unsigned int value3) const {
      glUniform3ui(Location(name), value1, value2, value3);
    }
    void Shader::SetUniform4UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                               unsigned int value3, unsigned int value4) const {
      glUniform4ui(Location(name), value1, value2, value3, value4);
    }

    void Shader::SetUniform1FV(const std::string_view& name, int count, float const* value) const {
      glUniform1fv(Location(name), count, value);
    }
    void Shader::SetUniform2FV(const std::string_view& name, int count, float const* value) const {
      glUniform2fv(Location(name), count, value);
    }
    void Shader::SetUniform3FV(const std::string_view& name, int count, float const* value) const {
      glUniform3fv(Location(name), count, value);
    }
    void Shader::SetUniform4FV(const std::string_view& name, int count, float const* value) const {
      glUniform4fv(Location(name), count, value);
    }
    void Shader::SetUniform1IV(const std::string_view& name, int count, int const* value) const {
      glUniform1iv(Location(name), count, value);
    }
    void Shader::SetUniform2IV(const std::string_view& name, int count, int const* value) const {
      glUniform2iv(Location(name), count, value);
    }
    void Shader::SetUniform3IV(const std::string_view& name, int count, int const* value) const {
      glUniform3iv(Location(name), count, value);
    }
    void Shader::SetUniform4IV(const std::string_view& name, int count, int const* value) const {
      glUniform4iv(Location(name), count, value);
    }
    void Shader::SetUniform1UIV(const std::string_view& name, int count, unsigned int const* value) const {
      glUniform1uiv(Location(name), count, value);
    }
    void Shader::SetUniform2UIV(const std::string_view& name, int count, unsigned int const* value) const {
      glUniform2uiv(Location(name), count, value);
    }
    void Shader::SetUniform3UIV(const std::string_view& name, int count, unsigned int const* value) const {
      glUniform3uiv(Location(name), count, value);
    }
    void Shader::SetUniform4UIV(const std::string_view& name, int count, unsigned int const* value) const {
      glUniform4uiv(Location(name), count, value);
    }

    void Shader::SetUniformMatrix2FV(const std::string_view& name, int count, bool transpose,
                                     float const* value) const {
      glUniformMatrix2fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix3FV(const std::string_view& name, int count, bool transpose,
                                     float const* value) const {
      glUniformMatrix3fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix4FV(const std::string_view& name, int count, bool transpose,
                                     float const* value) const {
      glUniformMatrix4fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix2x4FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix2x4fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix4x2FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix4x2fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix3x4FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix3x4fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix4x3FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix4x3fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix2x3FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix2x3fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }
    void Shader::SetUniformMatrix3x2FV(const std::string_view& name, int count, bool transpose,
                                       float const* value) const {
      glUniformMatrix3x2fv(Location(name), count, transpose ? GL_TRUE : GL_FALSE, value);
    }

  } // namespace OpenGL

} // namespace Chemical
