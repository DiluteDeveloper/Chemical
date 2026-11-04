#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

#include "misc.h"


namespace Chemical {


	namespace OpenGL {

		enum class ShaderType {
			VERTEX_SHADER = 0x8B31,
			FRAGMENT_SHADER = 0x8B30
			// geometry, tessellation and compute shaders don't exist in GLES 3.0 / WebGL2
		};

		// Shader source must be GLSL ES 3.00: first line "#version 300 es", plus a default
		// "precision highp float;" in the fragment shader.
		// Throws std::runtime_error (message = compile log) on failure.
		// Recommended to make a shared_ptr as copy constructor is disabled
		class Shader {
			uint32_t rendererID = 0;

			ShaderType type = ShaderType::VERTEX_SHADER;

			void Release();
		public:

			Shader(const char* data, ShaderType type);
			Shader(Shader&& other) noexcept :
				rendererID(std::exchange(other.rendererID, 0)), type(other.type) {}

			Shader(const Shader&) = delete;
			Shader& operator =(const Shader&) = delete;
			Shader& operator=(Shader&& other) noexcept {
				if (this != &other) {
					Release();
					rendererID = std::exchange(other.rendererID, 0);
					type = other.type;
				}
				return *this;
			}


			~Shader();

			uint32_t GetRendererID() const {
				return rendererID;
			}
			ShaderType GetShaderType() const {
				return type;
			}


		};

		struct Uniform {
			int32_t location = 0;

			Uniform(int32_t location) :
				location(location) {}
			Uniform() = default;
		};

		// Shaders must be added on creation, cannot be added later.
		// Throws std::runtime_error (message = link log) on failure.
		//
		// GLES 3.0 has no glProgramUniform*, so every setter makes this program current
		// (glUseProgram, skipped if it already is). Unknown uniform names are ignored
		// (location -1) instead of writing to location 0.
		class ShaderProgram {

			uint32_t rendererID = 0;

			std::unordered_map<std::string, Uniform> uniforms; // lazily filled location cache

			void Release();
			void Use();
			int32_t GetLocation(std::string_view name);

		public:
			ShaderProgram(std::initializer_list<const Shader*> shaders);

			ShaderProgram(ShaderProgram&& other) noexcept :
				rendererID(std::exchange(other.rendererID, 0)), uniforms(std::move(other.uniforms)){}

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram& operator =(const ShaderProgram&) = delete;
			ShaderProgram& operator=(ShaderProgram&& other) noexcept {
				if (this != &other) {
					Release();
					rendererID = std::exchange(other.rendererID, 0);
					uniforms = std::move(other.uniforms);
					// layout = std::move(other.layout);
				}
				return *this;
			}

			~ShaderProgram();

			void BindProgram();

			// GLSL ES 3.00 has no layout(binding = N) for uniform blocks, so assign it here and then
			// call Buffer::BindBufferBase(BufferBaseTarget::UNIFORM_BUFFER, bindingIndex).
			void SetUniformBlockBinding(std::string_view blockName, uint32_t bindingIndex);

			// Scalar setters. NOTE: sampler uniforms are set with SetUniform1I(name, textureUnit).
			void SetUniform1F(std::string_view name, float value);
			void SetUniform2F(std::string_view name, float value1, float value2);
			void SetUniform3F(std::string_view name, float value1, float value2, float value3);
			void SetUniform4F(std::string_view name, float value1, float value2, float value3, float value4);
			void SetUniform1I(std::string_view name, int32_t value);
			void SetUniform2I(std::string_view name, int32_t value1, int32_t value2);
			void SetUniform3I(std::string_view name, int32_t value1, int32_t value2, int32_t value3);
			void SetUniform4I(std::string_view name, int32_t value1, int32_t value2, int32_t value3, int32_t value4);
			void SetUniform1UI(std::string_view name, uint32_t value);
			void SetUniform2UI(std::string_view name, uint32_t value1, uint32_t value2);
			void SetUniform3UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3);
			void SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4);

			// count should be 1 unless it is an array, then count is array length.
			void SetUniform1FV(std::string_view name, int32_t count, float const* value);
			void SetUniform2FV(std::string_view name, int32_t count, float const* value);
			void SetUniform3FV(std::string_view name, int32_t count, float const* value);
			void SetUniform4FV(std::string_view name, int32_t count, float const* value);
			void SetUniform1IV(std::string_view name, int32_t count, int32_t const* value);
			void SetUniform2IV(std::string_view name, int32_t count, int32_t const* value);
			void SetUniform3IV(std::string_view name, int32_t count, int32_t const* value);
			void SetUniform4IV(std::string_view name, int32_t count, int32_t const* value);
			void SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value);
			void SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value);
			void SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value);
			void SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value);

			// count should be 1 unless it is an array, then count is array length.
			// transpose decides whether the matrix is flipped when sent to the shader
			void SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			void SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value);

			// Double-precision uniforms (SetUniform*D / SetUniformMatrix*DV) were removed: not in GLES 3.0.
		};
	}

}
