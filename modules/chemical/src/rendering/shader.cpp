#include "shader.hpp"

#include "spdlog/spdlog.h"
#include <cassert>
#include <glad/glad.h>

namespace Chemical {

  uint16_t Shader::CompileShader(const std::string_view &source, int type) {

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

    glLinkProgram(id);

    int success;
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
      unsigned int type = GL_NONE;
      glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

      auto uniform_name = std::make_unique<char[]>(max_name_len);

      for (GLint i = 0; i < uniform_count; ++i) {
        glGetActiveUniform(id, i, max_name_len, &length, &count, &type,
                           uniform_name.get());

        int location = glGetUniformLocation(id, uniform_name.get());
        uniform_locations[uniform_name.get()] = location;
      }
    }
    return true;
  }

  Shader::Shader(const ShaderTraits &traits) {

    uint16_t vs = 1, fs = 1, cs = 1, gs = 1;

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
    if (traits.cs_source != std::nullopt) {
      cs = CompileShader(traits.cs_source.value(), GL_COMPUTE_SHADER);
      if (cs != 0) {
        SPDLOG_INFO("Attaching compute shader to shader program");
        glAttachShader(id, cs);
      }

      glDeleteShader(cs);
    }
    if (traits.gs_source != std::nullopt) {
      gs = CompileShader(traits.gs_source.value(), GL_GEOMETRY_SHADER);
      if (gs != 0) {
        SPDLOG_INFO("Attaching geometry shader to shader program");
        glAttachShader(id, gs);
      }
      glDeleteShader(gs);
    }

    if (vs == 0 || fs == 0 || cs == 0 || gs == 0) {
      SPDLOG_ERROR("Terminating shader program finalisation and setting "
                   "compile_status to -1 : one or more "
                   "shaders failed to compile");
      compile_status = -1;
    } else {
      if (!FinaliseProgram()) {
        SPDLOG_ERROR("Setting compile_status to -1 : failed to finalise "
                     "shader program");
        compile_status = -1;
      }
    }
  }
  Shader::~Shader() {
    glDeleteProgram(id);
  }
  void Shader::Bind() {
    glUseProgram(id);
  }

  void Shader::SetUniform1F(const std::string_view &name, float value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform1f(id, uniform_locations.at(name.data()), (GLfloat)value);
  }
  void Shader::SetUniform2F(const std::string_view &name, float value1,
                            float value2) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform2f(id, uniform_locations.at(name.data()), (GLfloat)value1,
                       (GLfloat)value2);
  }
  void Shader::SetUniform3F(const std::string_view &name, float value1,
                            float value2, float value3) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform3f(id, uniform_locations.at(name.data()), (GLfloat)value1,
                       (GLfloat)value2, (GLfloat)value3);
  }
  void Shader::SetUniform4F(const std::string_view &name, float value1,
                            float value2, float value3, float value4) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform4f(id, uniform_locations.at(name.data()), (GLfloat)value1,
                       (GLfloat)value2, (GLfloat)value3, (GLfloat)value4);
  }
  void Shader::SetUniform1I(const std::string_view &name, int value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform1i(id, uniform_locations.at(name.data()), (GLint)value);
  }
  void Shader::SetUniform2I(const std::string_view &name, int value1,
                            int value2) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform2i(id, uniform_locations.at(name.data()), (GLint)value1,
                       (GLint)value2);
  }
  void Shader::SetUniform3I(const std::string_view &name, int value1,
                            int value2, int value3) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform3i(id, uniform_locations.at(name.data()), (GLint)value1,
                       (GLint)value2, (GLint)value3);
  }
  void Shader::SetUniform4I(const std::string_view &name, int value1,
                            int value2, int value3, int value4) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform4i(id, uniform_locations.at(name.data()), (GLint)value1,
                       (GLint)value2, (GLint)value3, (GLint)value4);
  }
  void Shader::SetUniform1UI(const std::string_view &name,
                             unsigned int value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform1ui(id, uniform_locations.at(name.data()), (GLuint)value);
  }
  void Shader::SetUniform2UI(const std::string_view &name, unsigned int value1,
                             unsigned int value2) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform2ui(id, uniform_locations.at(name.data()), (GLuint)value1,
                        (GLuint)value2);
  }
  void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1,
                             unsigned int value2, unsigned int value3) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform3ui(id, uniform_locations.at(name.data()), (GLuint)value1,
                        (GLuint)value2, (GLuint)value3);
  }
  void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1,
                             unsigned int value2, unsigned int value3,
                             unsigned int value4) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform4ui(id, uniform_locations.at(name.data()), (GLuint)value1,
                        (GLuint)value2, (GLuint)value3, (GLuint)value4);
  }

  void Shader::SetUniform1FV(const std::string_view &name, int count,
                             float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform1fv(id, uniform_locations.at(name.data()), count,
                        (GLfloat *)value);
  }
  void Shader::SetUniform2FV(const std::string_view &name, int count,
                             float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform2fv(id, uniform_locations.at(name.data()), count,
                        (GLfloat *)value);
  }
  void Shader::SetUniform3FV(const std::string_view &name, int count,
                             float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform3fv(id, uniform_locations.at(name.data()), count,
                        (GLfloat *)value);
  }
  void Shader::SetUniform4FV(const std::string_view &name, int count,
                             float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform4fv(id, uniform_locations.at(name.data()), count,
                        (GLfloat *)value);
  }
  void Shader::SetUniform1IV(const std::string_view &name, int count,
                             int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform1iv(id, uniform_locations.at(name.data()), count,
                        (GLint *)value);
  }
  void Shader::SetUniform2IV(const std::string_view &name, int count,
                             int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform2iv(id, uniform_locations.at(name.data()), count,
                        (GLint *)value);
  }
  void Shader::SetUniform3IV(const std::string_view &name, int count,
                             int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform3iv(id, uniform_locations.at(name.data()), count,
                        (GLint *)value);
  }
  void Shader::SetUniform4IV(const std::string_view &name, int count,
                             int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform4iv(id, uniform_locations.at(name.data()), count,
                        (GLint *)value);
  }
  void Shader::SetUniform1UIV(const std::string_view &name, int count,
                              unsigned int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform1uiv(id, uniform_locations.at(name.data()), count,
                         (GLuint *)value);
  }
  void Shader::SetUniform2UIV(const std::string_view &name, int count,
                              unsigned int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform2uiv(id, uniform_locations.at(name.data()), count,
                         (GLuint *)value);
  }
  void Shader::SetUniform3UIV(const std::string_view &name, int count,
                              unsigned int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform3uiv(id, uniform_locations.at(name.data()), count,
                         (GLuint *)value);
  }
  void Shader::SetUniform4UIV(const std::string_view &name, int count,
                              unsigned int const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform4uiv(id, uniform_locations.at(name.data()), count,
                         (GLuint *)value);
  }
  void Shader::SetUniformMatrix2FV(const std::string_view &name, int count,
                                   bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2fv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix3FV(const std::string_view &name, int count,
                                   bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3fv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix4FV(const std::string_view &name, int count,
                                   bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4fv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix2x4FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2x4fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix4x2FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4x2fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix3x4FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3x4fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix4x3FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4x3fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix2x3FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2x3fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }
  void Shader::SetUniformMatrix3x2FV(const std::string_view &name, int count,
                                     bool transpose, float const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3x2fv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLfloat *)value);
  }

  void Shader::SetUniform1D(const std::string_view &name, double value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform1d(id, uniform_locations.at(name.data()), (GLdouble)value);
  }
  void Shader::SetUniform2D(const std::string_view &name, double value1,
                            double value2) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform2d(id, uniform_locations.at(name.data()), (GLdouble)value1,
                       (GLdouble)value2);
  }
  void Shader::SetUniform3D(const std::string_view &name, double value1,
                            double value2, double value3) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform3d(id, uniform_locations.at(name.data()), (GLdouble)value1,
                       (GLdouble)value2, (GLdouble)value3);
  }
  void Shader::SetUniform4D(const std::string_view &name, double value1,
                            double value2, double value3, double value4) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");
    glProgramUniform4d(id, uniform_locations.at(name.data()), (GLdouble)value1,
                       (GLdouble)value2, (GLdouble)value3, (GLdouble)value4);
  }

  void Shader::SetUniform1DV(const std::string_view &name, int count,
                             double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform1dv(id, uniform_locations.at(name.data()), count,
                        (GLdouble *)value);
  }
  void Shader::SetUniform2DV(const std::string_view &name, int count,
                             double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform2dv(id, uniform_locations.at(name.data()), count,
                        (GLdouble *)value);
  }
  void Shader::SetUniform3DV(const std::string_view &name, int count,
                             double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform3dv(id, uniform_locations.at(name.data()), count,
                        (GLdouble *)value);
  }
  void Shader::SetUniform4DV(const std::string_view &name, int count,
                             double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniform4dv(id, uniform_locations.at(name.data()), count,
                        (GLdouble *)value);
  }
  void Shader::SetUniformMatrix2DV(const std::string_view &name, int count,
                                   bool transpose, double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2dv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix3DV(const std::string_view &name, int count,
                                   bool transpose, double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3dv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix4DV(const std::string_view &name, int count,
                                   bool transpose, double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4dv(id, uniform_locations.at(name.data()), count,
                              transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix2x4DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2x4dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix4x2DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4x2dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix3x4DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3x4dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix4x3DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix4x3dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix2x3DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix2x3dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }
  void Shader::SetUniformMatrix3x2DV(const std::string_view &name, int count,
                                     bool transpose,
                                     double const *value) const {
    assert(uniform_locations.contains(name.data()) && "Uniform does not exist");

    glProgramUniformMatrix3x2dv(id, uniform_locations.at(name.data()), count,
                                transpose, (GLdouble *)value);
  }

} // namespace Chemical
