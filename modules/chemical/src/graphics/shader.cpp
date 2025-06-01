#include "chemical/graphics/shader.h"

#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Chemical {
  namespace Graphics {
    unsigned int Shader::CompileShaderFromFile(const std::string_view &file_path, int type) {
      // Reading in source from file -------------------------------------

      SPDLOG_INFO(R"(Processing file "{}")", file_path);
      std::fstream file(file_path.data());

      if (!file) {
        SPDLOG_ERROR(R"(Failed to read file "{}" : returning 0)", file_path);
        return 0;
      }

      std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      file.close();

      // Reading in source from file -------------------------------------

      SPDLOG_INFO(R"(Creating and compiling OpenGL shader object "{}")", file_path);

      unsigned int shader = glCreateShader(type);

      const char *c_source = source.data();
      glShaderSource(shader, 1, &c_source, nullptr);

      glCompileShader(shader);

      int success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (success != GL_TRUE) {

        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, info_log);
        SPDLOG_ERROR(R"(Shader "{}" failed to compile : {})", source, info_log);

        return 0;
      }

      return shader;
    }
    bool Shader::FinaliseProgram() {

      SPDLOG_INFO(R"(Compiling shader program)");

      glLinkProgram(gl_id);

      int success;
      glGetProgramiv(gl_id, GL_LINK_STATUS, &success);
      if (success != GL_TRUE) {
        char info_log[1024];
        glGetProgramInfoLog(gl_id, 1024, nullptr, info_log);

        SPDLOG_ERROR(R"(Shader program failed to compile : {})", info_log);
        return false;
      }
      glValidateProgram(gl_id);

      SPDLOG_INFO(R"(Caching shader program uniform locations)");

      uniform_locations.clear();
      int uniform_count = 0;
      glGetProgramiv(gl_id, GL_ACTIVE_UNIFORMS, &uniform_count);

      if (uniform_count > 0) {
        int max_name_len = 0;
        int length = 0;
        int count = 0;
        unsigned int type = GL_NONE;
        glGetProgramiv(gl_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i) {
          glGetActiveUniform(gl_id, i, max_name_len, &length, &count, &type, uniform_name.get());

          int location = glGetUniformLocation(gl_id, uniform_name.get());
          uniform_locations[uniform_name.get()] = location;
        }
      }
      return true;
    }

    Shader::Shader(const ShaderTraits &traits) {

      unsigned int vs = 1, fs = 1, cs = 1, gs = 1;

      SPDLOG_INFO("Creating OpenGL shader program object");
      gl_id = glCreateProgram();

      if (traits.vs_file_path != "") {
        vs = CompileShaderFromFile(traits.vs_file_path, GL_VERTEX_SHADER);
        if (vs != 0) {
          SPDLOG_INFO("Attaching vertex shader to shader program");
          glAttachShader(gl_id, vs);
        }
        glDeleteShader(vs);
      }
      if (traits.fs_file_path != "") {
        fs = CompileShaderFromFile(traits.fs_file_path, GL_FRAGMENT_SHADER);
        if (fs != 0) {
          SPDLOG_INFO("Attaching fragment shader to shader program");
          glAttachShader(gl_id, fs);
        }
        glDeleteShader(fs);
      }
      if (traits.cs_file_path != "") {
        cs = CompileShaderFromFile(traits.cs_file_path, GL_COMPUTE_SHADER);
        if (cs != 0) {
          SPDLOG_INFO("Attaching compute shader to shader program");
          glAttachShader(gl_id, cs);
        }

        glDeleteShader(cs);
      }
      if (traits.gs_file_path != "") {
        gs = CompileShaderFromFile(traits.gs_file_path, GL_GEOMETRY_SHADER);
        if (gs != 0) {
          SPDLOG_INFO("Attaching geometry shader to shader program");
          glAttachShader(gl_id, gs);
        }
        glDeleteShader(gs);
      }

      if (vs == 0 || fs == 0 || cs == 0 || gs == 0) {
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

    void Shader::Bind() const {
      glUseProgram(gl_id);
    }
    Shader::~Shader() {
      SPDLOG_INFO("Deleting OpenGL shader program {}", gl_id);
      glDeleteProgram(gl_id);
    }

    void Shader::SetUniform1F(const std::string_view &name, float value) const {
      glProgramUniform1f(gl_id, uniform_locations.at(name.data()), (GLfloat)value);
    }
    void Shader::SetUniform2F(const std::string_view &name, float value1, float value2) const {
      glProgramUniform2f(gl_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2);
    }
    void Shader::SetUniform3F(const std::string_view &name, float value1, float value2, float value3) const {
      glProgramUniform3f(gl_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2,
                         (GLfloat)value3);
    }
    void Shader::SetUniform4F(const std::string_view &name, float value1, float value2, float value3,
                              float value4) const {
      glProgramUniform4f(gl_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2,
                         (GLfloat)value3, (GLfloat)value4);
    }
    void Shader::SetUniform1I(const std::string_view &name, int value) const {
      glProgramUniform1i(gl_id, uniform_locations.at(name.data()), (GLint)value);
    }
    void Shader::SetUniform2I(const std::string_view &name, int value1, int value2) const {
      glProgramUniform2i(gl_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2);
    }
    void Shader::SetUniform3I(const std::string_view &name, int value1, int value2, int value3) const {
      glProgramUniform3i(gl_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2,
                         (GLint)value3);
    }
    void Shader::SetUniform4I(const std::string_view &name, int value1, int value2, int value3,
                              int value4) const {
      glProgramUniform4i(gl_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2,
                         (GLint)value3, (GLint)value4);
    }
    void Shader::SetUniform1UI(const std::string_view &name, unsigned int value) const {
      glProgramUniform1ui(gl_id, uniform_locations.at(name.data()), (GLuint)value);
    }
    void Shader::SetUniform2UI(const std::string_view &name, unsigned int value1, unsigned int value2) const {
      glProgramUniform2ui(gl_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2);
    }
    void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1, unsigned int value2,
                               unsigned int value3) const {
      glProgramUniform3ui(gl_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2,
                          (GLuint)value3);
    }
    void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1, unsigned int value2,
                               unsigned int value3, unsigned int value4) const {
      glProgramUniform4ui(gl_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2,
                          (GLuint)value3, (GLuint)value4);
    }

    void Shader::SetUniform1FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform1fv(gl_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform2FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform2fv(gl_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform3FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform3fv(gl_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform4FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform4fv(gl_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform1IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform1iv(gl_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform2IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform2iv(gl_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform3IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform3iv(gl_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform4IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform4iv(gl_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform1UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform1uiv(gl_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform2UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform2uiv(gl_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform3UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform3uiv(gl_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform4UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform4uiv(gl_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniformMatrix2FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix2fv(gl_id, uniform_locations.at(name.data()), count, transpose, (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix3fv(gl_id, uniform_locations.at(name.data()), count, transpose, (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix4fv(gl_id, uniform_locations.at(name.data()), count, transpose, (GLfloat *)value);
    }
    void Shader::SetUniformMatrix2x4FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix2x4fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4x2FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix4x2fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3x4FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix3x4fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4x3FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix4x3fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix2x3FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix2x3fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3x2FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix3x2fv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }

    void Shader::SetUniform1D(const std::string_view &name, double value) const {
      glProgramUniform1d(gl_id, uniform_locations.at(name.data()), (GLdouble)value);
    }
    void Shader::SetUniform2D(const std::string_view &name, double value1, double value2) const {
      glProgramUniform2d(gl_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2);
    }
    void Shader::SetUniform3D(const std::string_view &name, double value1, double value2,
                              double value3) const {
      glProgramUniform3d(gl_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2,
                         (GLdouble)value3);
    }
    void Shader::SetUniform4D(const std::string_view &name, double value1, double value2, double value3,
                              double value4) const {
      glProgramUniform4d(gl_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2,
                         (GLdouble)value3, (GLdouble)value4);
    }

    void Shader::SetUniform1DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform1dv(gl_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform2DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform2dv(gl_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform3DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform3dv(gl_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform4DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform4dv(gl_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix2dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix3dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix4dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2x4DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix2x4dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4x2DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix4x2dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3x4DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix3x4dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4x3DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix4x3dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2x3DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix2x3dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3x2DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix3x2dv(gl_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }

  } // namespace Graphics
} // namespace Chemical
