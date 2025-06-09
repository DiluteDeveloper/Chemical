#pragma once

#include "chemical/graphics/shader_traits.h"

#include <cstdint>
#include <unordered_map>

namespace Chemical {

  class GL_Shader {
  public:
    GL_Shader() = default;

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
    void SetUniform4UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                       unsigned int value3) const;
    void SetUniform4UI(const std::string_view& name, unsigned int value1, unsigned int value2,
                       unsigned int value3, unsigned int value4) const;

    // count should be 1 unless it is an array, then count is array length.
    void SetUniform1FV(const std::string_view& name, int count, float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform2FV(const std::string_view& name, int count, float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform3FV(const std::string_view& name, int count, float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform4FV(const std::string_view& name, int count, float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform1IV(const std::string_view& name, int count, int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform2IV(const std::string_view& name, int count, int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform3IV(const std::string_view& name, int count, int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform4IV(const std::string_view& name, int count, int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform1UIV(const std::string_view& name, int count, unsigned int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform2UIV(const std::string_view& name, int count, unsigned int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform3UIV(const std::string_view& name, int count, unsigned int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform4UIV(const std::string_view& name, int count, unsigned int const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniformMatrix2FV(const std::string_view& name, int count, bool transpose,
                             float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3FV(const std::string_view& name, int count, bool transpose,
                             float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4FV(const std::string_view& name, int count, bool transpose,
                             float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix2x4FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4x2FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3x4FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4x3FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix2x3FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3x2FV(const std::string_view& name, int count, bool transpose,
                               float const* value) const;

    void SetUniform1D(const std::string_view& name, double value) const;
    void SetUniform2D(const std::string_view& name, double value1, double value2) const;
    void SetUniform3D(const std::string_view& name, double value1, double value2, double value3) const;
    void SetUniform4D(const std::string_view& name, double value1, double value2, double value3,
                      double value4) const;

    // count should be 1 unless it is an array, then count is array length.
    void SetUniform1DV(const std::string_view& name, int count, double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform2DV(const std::string_view& name, int count, double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform3DV(const std::string_view& name, int count, double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniform4DV(const std::string_view& name, int count, double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    void SetUniformMatrix2DV(const std::string_view& name, int count, bool transpose,
                             double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3DV(const std::string_view& name, int count, bool transpose,
                             double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4DV(const std::string_view& name, int count, bool transpose,
                             double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix2x4DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4x2DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3x4DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix4x3DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix2x3DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;
    // count should be 1 unless it is an array, then count is array length.
    // transpose will decide whether the matrix is flipped when sent to the
    // shader
    void SetUniformMatrix3x2DV(const std::string_view& name, int count, bool transpose,
                               double const* value) const;

  protected:
    friend class GL_Renderer;

    uint16_t id = 0;

    std::unordered_map<std::string, int> uniform_locations;

    bool FinaliseProgram();

    uint16_t CompileShader(const std::string_view& source, int type);

    void CreateGLShader(const ShaderTraits& traits);
    void DeleteGLShader();
  };
} // namespace Chemical
