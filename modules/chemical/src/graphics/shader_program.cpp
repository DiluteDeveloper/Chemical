#include "shader_program.h"

#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <stdexcept>
namespace Chemical {
  namespace Graphics {

    unsigned int BuildShaderFromSourceFile(const std::string_view &file_path,
                                           GLenum type) {
      // Reading in source from file -------------------------------------

      std::fstream file(file_path.data());

      if (!file) {
        spdlog::error("File \"{}\" failed to open.", file_path);
        return 0;
      }

      std::string source((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

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
        spdlog::error("Shader type {} with source \"{}\" failed to compile: {}",
                      type, source, info_log);

        return 0;
      }

      return shader;
    }
    unsigned int ShaderProgram::GetStatus() { return renderer_id; }
    void ShaderProgram::FinaliseProgram() {
      assert(renderer_id != 0);

      glLinkProgram(renderer_id);

      int success;
      glGetProgramiv(renderer_id, GL_LINK_STATUS, &success);
      if (success != GL_TRUE) {
        char infoLog[1024];
        glGetProgramInfoLog(renderer_id, 1024, nullptr, infoLog);

        spdlog::error("ShaderProgram compilation failed. \n{}", infoLog);

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
        glGetProgramiv(renderer_id, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                       &max_name_len);

        auto uniform_name = std::make_unique<char[]>(max_name_len);

        for (GLint i = 0; i < uniform_count; ++i) {
          glGetActiveUniform(renderer_id, i, max_name_len, &length, &count,
                             &type, uniform_name.get());

          int location = glGetUniformLocation(renderer_id, uniform_name.get());
          uniform_locations[uniform_name.get()] = location;
        }
      }
    }

    ShaderProgram::ShaderProgram(const ShaderSourceFiles &ssf) {

      assert(!(ssf.cs_file_path == "" && ssf.fs_file_path == "" &&
               ssf.gs_file_path == "" && ssf.vs_file_path == ""));

      unsigned int vs = 1, fs = 1, cs = 1, gs = 1;

      renderer_id = glCreateProgram();

      if (ssf.vs_file_path != "") {
        vs = BuildShaderFromSourceFile(ssf.vs_file_path, GL_VERTEX_SHADER);
        if (vs != 0)
          glAttachShader(renderer_id, vs);
        glDeleteShader(vs);
      }
      if (ssf.fs_file_path != "") {
        fs = BuildShaderFromSourceFile(ssf.fs_file_path, GL_FRAGMENT_SHADER);
        if (fs != 0)
          glAttachShader(renderer_id, fs);
        glDeleteShader(fs);
      }
      if (ssf.cs_file_path != "") {
        cs = BuildShaderFromSourceFile(ssf.cs_file_path, GL_COMPUTE_SHADER);
        if (cs != 0)
          glAttachShader(renderer_id, cs);
        glDeleteShader(cs);
      }
      if (ssf.gs_file_path != "") {
        gs = BuildShaderFromSourceFile(ssf.gs_file_path, GL_GEOMETRY_SHADER);
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

    void ShaderProgram::Bind() {
      assert(renderer_id != 0);

      glUseProgram(renderer_id);
    }
    ShaderProgram::~ShaderProgram() { glDeleteProgram(renderer_id); }

    void ShaderProgram::SetUniform1F(const std::string_view &name,
                                     float value) {
      glProgramUniform1f(renderer_id, uniform_locations[name.data()],
                         (GLfloat)value);
    }
    void ShaderProgram::SetUniform2F(const std::string_view &name, float value1,
                                     float value2) {
      glProgramUniform2f(renderer_id, uniform_locations[name.data()],
                         (GLfloat)value1, (GLfloat)value2);
    }
    void ShaderProgram::SetUniform3F(const std::string_view &name, float value1,
                                     float value2, float value3) {
      glProgramUniform3f(renderer_id, uniform_locations[name.data()],
                         (GLfloat)value1, (GLfloat)value2, (GLfloat)value3);
    }
    void ShaderProgram::SetUniform4F(const std::string_view &name, float value1,
                                     float value2, float value3, float value4) {
      glProgramUniform4f(renderer_id, uniform_locations[name.data()],
                         (GLfloat)value1, (GLfloat)value2, (GLfloat)value3,
                         (GLfloat)value4);
    }
    void ShaderProgram::SetUniform1I(const std::string_view &name, int value) {
      glProgramUniform1i(renderer_id, uniform_locations[name.data()],
                         (GLint)value);
    }
    void ShaderProgram::SetUniform2I(const std::string_view &name, int value1,
                                     int value2) {
      glProgramUniform2i(renderer_id, uniform_locations[name.data()],
                         (GLint)value1, (GLint)value2);
    }
    void ShaderProgram::SetUniform3I(const std::string_view &name, int value1,
                                     int value2, int value3) {
      glProgramUniform3i(renderer_id, uniform_locations[name.data()],
                         (GLint)value1, (GLint)value2, (GLint)value3);
    }
    void ShaderProgram::SetUniform4I(const std::string_view &name, int value1,
                                     int value2, int value3, int value4) {
      glProgramUniform4i(renderer_id, uniform_locations[name.data()],
                         (GLint)value1, (GLint)value2, (GLint)value3,
                         (GLint)value4);
    }
    void ShaderProgram::SetUniform1UI(const std::string_view &name,
                                      unsigned int value) {
      glProgramUniform1ui(renderer_id, uniform_locations[name.data()],
                          (GLuint)value);
    }
    void ShaderProgram::SetUniform2UI(const std::string_view &name,
                                      unsigned int value1,
                                      unsigned int value2) {
      glProgramUniform2ui(renderer_id, uniform_locations[name.data()],
                          (GLuint)value1, (GLuint)value2);
    }
    void ShaderProgram::SetUniform4UI(const std::string_view &name,
                                      unsigned int value1, unsigned int value2,
                                      unsigned int value3) {
      glProgramUniform3ui(renderer_id, uniform_locations[name.data()],
                          (GLuint)value1, (GLuint)value2, (GLuint)value3);
    }
    void ShaderProgram::SetUniform4UI(const std::string_view &name,
                                      unsigned int value1, unsigned int value2,
                                      unsigned int value3,
                                      unsigned int value4) {
      glProgramUniform4ui(renderer_id, uniform_locations[name.data()],
                          (GLuint)value1, (GLuint)value2, (GLuint)value3,
                          (GLuint)value4);
    }

    void ShaderProgram::SetUniform1FV(const std::string_view &name, int count,
                                      float const *value) {

      glProgramUniform1fv(renderer_id, uniform_locations[name.data()], count,
                          (GLfloat *)value);
    }
    void ShaderProgram::SetUniform2FV(const std::string_view &name, int count,
                                      float const *value) {

      glProgramUniform2fv(renderer_id, uniform_locations[name.data()], count,
                          (GLfloat *)value);
    }
    void ShaderProgram::SetUniform3FV(const std::string_view &name, int count,
                                      float const *value) {

      glProgramUniform3fv(renderer_id, uniform_locations[name.data()], count,
                          (GLfloat *)value);
    }
    void ShaderProgram::SetUniform4FV(const std::string_view &name, int count,
                                      float const *value) {

      glProgramUniform4fv(renderer_id, uniform_locations[name.data()], count,
                          (GLfloat *)value);
    }
    void ShaderProgram::SetUniform1IV(const std::string_view &name, int count,
                                      int const *value) {

      glProgramUniform1iv(renderer_id, uniform_locations[name.data()], count,
                          (GLint *)value);
    }
    void ShaderProgram::SetUniform2IV(const std::string_view &name, int count,
                                      int const *value) {

      glProgramUniform2iv(renderer_id, uniform_locations[name.data()], count,
                          (GLint *)value);
    }
    void ShaderProgram::SetUniform3IV(const std::string_view &name, int count,
                                      int const *value) {

      glProgramUniform3iv(renderer_id, uniform_locations[name.data()], count,
                          (GLint *)value);
    }
    void ShaderProgram::SetUniform4IV(const std::string_view &name, int count,
                                      int const *value) {

      glProgramUniform4iv(renderer_id, uniform_locations[name.data()], count,
                          (GLint *)value);
    }
    void ShaderProgram::SetUniform1UIV(const std::string_view &name, int count,
                                       unsigned int const *value) {

      glProgramUniform1uiv(renderer_id, uniform_locations[name.data()], count,
                           (GLuint *)value);
    }
    void ShaderProgram::SetUniform2UIV(const std::string_view &name, int count,
                                       unsigned int const *value) {

      glProgramUniform2uiv(renderer_id, uniform_locations[name.data()], count,
                           (GLuint *)value);
    }
    void ShaderProgram::SetUniform3UIV(const std::string_view &name, int count,
                                       unsigned int const *value) {

      glProgramUniform3uiv(renderer_id, uniform_locations[name.data()], count,
                           (GLuint *)value);
    }
    void ShaderProgram::SetUniform4UIV(const std::string_view &name, int count,
                                       unsigned int const *value) {

      glProgramUniform4uiv(renderer_id, uniform_locations[name.data()], count,
                           (GLuint *)value);
    }
    void ShaderProgram::SetUniformMatrix2FV(const std::string_view &name,
                                            int count, bool transpose,
                                            float const *value) {

      glProgramUniformMatrix2fv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix3FV(const std::string_view &name,
                                            int count, bool transpose,
                                            float const *value) {

      glProgramUniformMatrix3fv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix4FV(const std::string_view &name,
                                            int count, bool transpose,
                                            float const *value) {

      glProgramUniformMatrix4fv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix2x4FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix2x4fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix4x2FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix4x2fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix3x4FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix3x4fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix4x3FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix4x3fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix2x3FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix2x3fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }
    void ShaderProgram::SetUniformMatrix3x2FV(const std::string_view &name,
                                              int count, bool transpose,
                                              float const *value) {

      glProgramUniformMatrix3x2fv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLfloat *)value);
    }

    void ShaderProgram::SetUniform1D(const std::string_view &name,
                                     double value) {
      glProgramUniform1d(renderer_id, uniform_locations[name.data()],
                         (GLdouble)value);
    }
    void ShaderProgram::SetUniform2D(const std::string_view &name,
                                     double value1, double value2) {
      glProgramUniform2d(renderer_id, uniform_locations[name.data()],
                         (GLdouble)value1, (GLdouble)value2);
    }
    void ShaderProgram::SetUniform3D(const std::string_view &name,
                                     double value1, double value2,
                                     double value3) {
      glProgramUniform3d(renderer_id, uniform_locations[name.data()],
                         (GLdouble)value1, (GLdouble)value2, (GLdouble)value3);
    }
    void ShaderProgram::SetUniform4D(const std::string_view &name,
                                     double value1, double value2,
                                     double value3, double value4) {
      glProgramUniform4d(renderer_id, uniform_locations[name.data()],
                         (GLdouble)value1, (GLdouble)value2, (GLdouble)value3,
                         (GLdouble)value4);
    }

    void ShaderProgram::SetUniform1DV(const std::string_view &name, int count,
                                      double const *value) {

      glProgramUniform1dv(renderer_id, uniform_locations[name.data()], count,
                          (GLdouble *)value);
    }
    void ShaderProgram::SetUniform2DV(const std::string_view &name, int count,
                                      double const *value) {

      glProgramUniform2dv(renderer_id, uniform_locations[name.data()], count,
                          (GLdouble *)value);
    }
    void ShaderProgram::SetUniform3DV(const std::string_view &name, int count,
                                      double const *value) {

      glProgramUniform3dv(renderer_id, uniform_locations[name.data()], count,
                          (GLdouble *)value);
    }
    void ShaderProgram::SetUniform4DV(const std::string_view &name, int count,
                                      double const *value) {

      glProgramUniform4dv(renderer_id, uniform_locations[name.data()], count,
                          (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix2DV(const std::string_view &name,
                                            int count, bool transpose,
                                            double const *value) {

      glProgramUniformMatrix2dv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix3DV(const std::string_view &name,
                                            int count, bool transpose,
                                            double const *value) {

      glProgramUniformMatrix3dv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix4DV(const std::string_view &name,
                                            int count, bool transpose,
                                            double const *value) {

      glProgramUniformMatrix4dv(renderer_id, uniform_locations[name.data()],
                                count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix2x4DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix2x4dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix4x2DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix4x2dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix3x4DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix3x4dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix4x3DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix4x3dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix2x3DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix2x3dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
    void ShaderProgram::SetUniformMatrix3x2DV(const std::string_view &name,
                                              int count, bool transpose,
                                              double const *value) {

      glProgramUniformMatrix3x2dv(renderer_id, uniform_locations[name.data()],
                                  count, transpose, (GLdouble *)value);
    }
  } // namespace Graphics
} // namespace Chemical
