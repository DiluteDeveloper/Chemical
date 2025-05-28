#include "chemical/graphics/shader.h"

#include "resources.h"

#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Chemical {
  namespace Graphics {
    unsigned int BuildShaderFromSourceFile(const std::string_view &file_path, GLenum type) {
      // Reading in source from file -------------------------------------

      std::fstream file(file_path.data());

      if (!file) {
        spdlog::error("File \"{}\" failed to open.", file_path);
        return 0;
      }

      std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

      file.close();

      // Reading in source from file -------------------------------------

      unsigned int shader = glCreateShader(type);

      const char *c_source = source.data();
      glShaderSource(shader, 1, &c_source, nullptr);

      glCompileShader(shader);

      int success;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (success != GL_TRUE) {

        char info_log[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, info_log);
        spdlog::error("Shader type {} with source \"{}\" failed to compile: {}", type, source, info_log);

        return 0;
      }

      return shader;
    }
    unsigned int Shader::GetCompileStatus() { return renderer_id; }
    void Shader::FinaliseProgram() {
      assert(renderer_id != 0);

      glLinkProgram(renderer_id);

      int success;
      glGetProgramiv(renderer_id, GL_LINK_STATUS, &success);
      if (success != GL_TRUE) {
        char infoLog[1024];
        glGetProgramInfoLog(renderer_id, 1024, nullptr, infoLog);

        spdlog::error("Shader compilation failed. \n{}", infoLog);

        throw std::runtime_error("Exception occurred. see console log.");
      }
      glValidateProgram(renderer_id);

      uniform_locations.clear();
      int uniform_count = 0;
      glGetProgramiv(renderer_id, GL_ACTIVE_UNIFORMS, &uniform_count);

      if (uniform_count > 0) {
        int max_name_len = 0;
        int length = 0;
        int count = 0;
        unsigned int type = GL_NONE;
        glGetProgramiv(renderer_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i) {
          glGetActiveUniform(renderer_id, i, max_name_len, &length, &count, &type, uniform_name.get());

          int location = glGetUniformLocation(renderer_id, uniform_name.get());
          uniform_locations[uniform_name.get()] = location;
        }
      }
    }

    Shader::Shader(const ShaderTraits &traits) {

      assert(!(traits.cs_file_path == "" && traits.fs_file_path == "" && traits.gs_file_path == "" &&
               traits.vs_file_path == ""));

      unsigned int vs = 1, fs = 1, cs = 1, gs = 1;

      renderer_id = glCreateProgram();

      if (traits.vs_file_path != "") {
        vs = BuildShaderFromSourceFile(traits.vs_file_path, GL_VERTEX_SHADER);
        if (vs != 0)
          glAttachShader(renderer_id, vs);
        glDeleteShader(vs);
      }
      if (traits.fs_file_path != "") {
        fs = BuildShaderFromSourceFile(traits.fs_file_path, GL_FRAGMENT_SHADER);
        if (fs != 0)
          glAttachShader(renderer_id, fs);
        glDeleteShader(fs);
      }
      if (traits.cs_file_path != "") {
        cs = BuildShaderFromSourceFile(traits.cs_file_path, GL_COMPUTE_SHADER);
        if (cs != 0)
          glAttachShader(renderer_id, cs);
        glDeleteShader(cs);
      }
      if (traits.gs_file_path != "") {
        gs = BuildShaderFromSourceFile(traits.gs_file_path, GL_GEOMETRY_SHADER);
        if (gs != 0) {
          glAttachShader(renderer_id, gs);
          glDeleteShader(gs);
        }
      }

      if (vs == 0 || fs == 0 || cs == 0 || gs == 0) {
        spdlog::error("Failed to create shader program.");
        glDeleteProgram(renderer_id);
        renderer_id = 0;
      } else {
        FinaliseProgram();
      }
    }

    void Shader::Bind() const {
      assert(renderer_id != 0);

      glUseProgram(renderer_id);
    }
    Shader::~Shader() { glDeleteProgram(renderer_id); }

    void Shader::SetUniform1F(const std::string_view &name, float value) const {
      glProgramUniform1f(renderer_id, uniform_locations.at(name.data()), (GLfloat)value);
    }
    void Shader::SetUniform2F(const std::string_view &name, float value1, float value2) const {
      glProgramUniform2f(renderer_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2);
    }
    void Shader::SetUniform3F(const std::string_view &name, float value1, float value2, float value3) const {
      glProgramUniform3f(renderer_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2,
                         (GLfloat)value3);
    }
    void Shader::SetUniform4F(const std::string_view &name, float value1, float value2, float value3,
                              float value4) const {
      glProgramUniform4f(renderer_id, uniform_locations.at(name.data()), (GLfloat)value1, (GLfloat)value2,
                         (GLfloat)value3, (GLfloat)value4);
    }
    void Shader::SetUniform1I(const std::string_view &name, int value) const {
      glProgramUniform1i(renderer_id, uniform_locations.at(name.data()), (GLint)value);
    }
    void Shader::SetUniform2I(const std::string_view &name, int value1, int value2) const {
      glProgramUniform2i(renderer_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2);
    }
    void Shader::SetUniform3I(const std::string_view &name, int value1, int value2, int value3) const {
      glProgramUniform3i(renderer_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2,
                         (GLint)value3);
    }
    void Shader::SetUniform4I(const std::string_view &name, int value1, int value2, int value3,
                              int value4) const {
      glProgramUniform4i(renderer_id, uniform_locations.at(name.data()), (GLint)value1, (GLint)value2,
                         (GLint)value3, (GLint)value4);
    }
    void Shader::SetUniform1UI(const std::string_view &name, unsigned int value) const {
      glProgramUniform1ui(renderer_id, uniform_locations.at(name.data()), (GLuint)value);
    }
    void Shader::SetUniform2UI(const std::string_view &name, unsigned int value1, unsigned int value2) const {
      glProgramUniform2ui(renderer_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2);
    }
    void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1, unsigned int value2,
                               unsigned int value3) const {
      glProgramUniform3ui(renderer_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2,
                          (GLuint)value3);
    }
    void Shader::SetUniform4UI(const std::string_view &name, unsigned int value1, unsigned int value2,
                               unsigned int value3, unsigned int value4) const {
      glProgramUniform4ui(renderer_id, uniform_locations.at(name.data()), (GLuint)value1, (GLuint)value2,
                          (GLuint)value3, (GLuint)value4);
    }

    void Shader::SetUniform1FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform1fv(renderer_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform2FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform2fv(renderer_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform3FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform3fv(renderer_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform4FV(const std::string_view &name, int count, float const *value) const {

      glProgramUniform4fv(renderer_id, uniform_locations.at(name.data()), count, (GLfloat *)value);
    }
    void Shader::SetUniform1IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform1iv(renderer_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform2IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform2iv(renderer_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform3IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform3iv(renderer_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform4IV(const std::string_view &name, int count, int const *value) const {

      glProgramUniform4iv(renderer_id, uniform_locations.at(name.data()), count, (GLint *)value);
    }
    void Shader::SetUniform1UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform1uiv(renderer_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform2UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform2uiv(renderer_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform3UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform3uiv(renderer_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniform4UIV(const std::string_view &name, int count, unsigned int const *value) const {

      glProgramUniform4uiv(renderer_id, uniform_locations.at(name.data()), count, (GLuint *)value);
    }
    void Shader::SetUniformMatrix2FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix2fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix3fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4FV(const std::string_view &name, int count, bool transpose,
                                     float const *value) const {

      glProgramUniformMatrix4fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLfloat *)value);
    }
    void Shader::SetUniformMatrix2x4FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix2x4fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4x2FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix4x2fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3x4FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix3x4fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix4x3FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix4x3fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix2x3FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix2x3fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }
    void Shader::SetUniformMatrix3x2FV(const std::string_view &name, int count, bool transpose,
                                       float const *value) const {

      glProgramUniformMatrix3x2fv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLfloat *)value);
    }

    void Shader::SetUniform1D(const std::string_view &name, double value) const {
      glProgramUniform1d(renderer_id, uniform_locations.at(name.data()), (GLdouble)value);
    }
    void Shader::SetUniform2D(const std::string_view &name, double value1, double value2) const {
      glProgramUniform2d(renderer_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2);
    }
    void Shader::SetUniform3D(const std::string_view &name, double value1, double value2,
                              double value3) const {
      glProgramUniform3d(renderer_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2,
                         (GLdouble)value3);
    }
    void Shader::SetUniform4D(const std::string_view &name, double value1, double value2, double value3,
                              double value4) const {
      glProgramUniform4d(renderer_id, uniform_locations.at(name.data()), (GLdouble)value1, (GLdouble)value2,
                         (GLdouble)value3, (GLdouble)value4);
    }

    void Shader::SetUniform1DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform1dv(renderer_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform2DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform2dv(renderer_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform3DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform3dv(renderer_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniform4DV(const std::string_view &name, int count, double const *value) const {

      glProgramUniform4dv(renderer_id, uniform_locations.at(name.data()), count, (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix2dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix3dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4DV(const std::string_view &name, int count, bool transpose,
                                     double const *value) const {

      glProgramUniformMatrix4dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2x4DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix2x4dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4x2DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix4x2dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3x4DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix3x4dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix4x3DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix4x3dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix2x3DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix2x3dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }
    void Shader::SetUniformMatrix3x2DV(const std::string_view &name, int count, bool transpose,
                                       double const *value) const {

      glProgramUniformMatrix3x2dv(renderer_id, uniform_locations.at(name.data()), count, transpose,
                                  (GLdouble *)value);
    }

  } // namespace Graphics
} // namespace Chemical
