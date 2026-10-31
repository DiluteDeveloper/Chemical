#pragma once

#include "rendering/shader.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Chemical {

  namespace OpenGL {

    // OpenGL ES 3.0 shader program.
    //
    // ES 3.0 has no glProgramUniform*, so every SetUniform* call uploads to the
    // currently bound program. Call Bind() before setting uniforms.
    //
    // Not available in ES 3.0 / WebGL2: double uniforms, compute shaders and
    // geometry shaders.
    class Shader {
    public:
      Shader() = default;

      int compile_status = 0;

      void Bind() const;

      void SetUniform1F(const std::string_view& name, float value) const;
      void SetUniform2F(const std::string_view& name, float value1, float value2) const;
      void SetUniform3F(const std::string_view& name, float value1, float value2, float value3) const;
      void SetUniform4F(const std::string_view& name, float value1, float value2, float value3,
                        float value4) const;
      void SetUniform1I(const std::string_view& name, int value) const;
      void SetUniform2I(const std::string_view& name, int value1, int value2) const;
      void SetUniform3I(const std::string_view& name, int value1, int value2, int value3) const;
      void SetUniform4I(const std::string_view& name, int value1, int value2, int value3, int value4) const;
      void SetUniform1UI(const std::string_view& name, unsigned int value) const;
      void SetUniform2UI(const std::string_view& name, unsigned int value1, unsigned int value2) const;
      void SetUniform3UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                         unsigned int value3) const;
      void SetUniform4UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                         unsigned int value3, unsigned int value4) const;

      // For the vector setters below, count should be 1 unless the uniform is
      // an array, then count is the array length.
      void SetUniform1FV(const std::string_view& name, int count, float const* value) const;
      void SetUniform2FV(const std::string_view& name, int count, float const* value) const;
      void SetUniform3FV(const std::string_view& name, int count, float const* value) const;
      void SetUniform4FV(const std::string_view& name, int count, float const* value) const;
      void SetUniform1IV(const std::string_view& name, int count, int const* value) const;
      void SetUniform2IV(const std::string_view& name, int count, int const* value) const;
      void SetUniform3IV(const std::string_view& name, int count, int const* value) const;
      void SetUniform4IV(const std::string_view& name, int count, int const* value) const;
      void SetUniform1UIV(const std::string_view& name, int count, unsigned int const* value) const;
      void SetUniform2UIV(const std::string_view& name, int count, unsigned int const* value) const;
      void SetUniform3UIV(const std::string_view& name, int count, unsigned int const* value) const;
      void SetUniform4UIV(const std::string_view& name, int count, unsigned int const* value) const;

      // For the matrix setters below, count should be 1 unless the uniform is
      // an array, then count is the array length. transpose decides whether
      // the matrix is flipped when sent to the shader (allowed in ES 3.0).
      void SetUniformMatrix2FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
      void SetUniformMatrix3FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
      void SetUniformMatrix4FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
      void SetUniformMatrix2x4FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;
      void SetUniformMatrix4x2FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;
      void SetUniformMatrix3x4FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;
      void SetUniformMatrix4x3FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;
      void SetUniformMatrix2x3FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;
      void SetUniformMatrix3x2FV(const std::string_view& name, int count, bool transpose,
                                 float const* value) const;

      uint32_t id = 0;

      std::unordered_map<std::string, int> uniform_locations;

      bool FinaliseProgram();

      uint32_t CompileShader(const std::string_view& source, int type);

      void CreateGLShader(const ShaderTraits& traits);
      void DeleteGLShader() const;

    private:
      // Returns -1 for unknown uniforms (for example ones the GLSL compiler
      // optimised away). glUniform* silently ignores location -1.
      int Location(const std::string_view& name) const;
    };
  } // namespace OpenGL
} // namespace Chemical
