#pragma once

#include "misc.h"


namespace Chemical {


	namespace OpenGL {

		enum class ShaderType {
			VERTEX_SHADER = 0x8B31,
			FRAGMENT_SHADER = 0x8B30,
			GEOMETRY_SHADER = 0x8DD9,
			COMPUTE_SHADER = 0x91B9,
			TESS_CONTROL_SHADER = 0x8E88,
			TESS_EVALUATION_SHADER = 0x8E87
		};

		// Recommended to make a shared_ptr as copy constructor is disabled
		class Shader {
			uint32_t renderer_id = 0;

			ShaderType type = ShaderType::VERTEX_SHADER;

		public:

			Shader(const char* data, ShaderType type);
			Shader(Shader&& other) noexcept :
				renderer_id(std::move(other.renderer_id)), type(std::move(other.type)) {}

			Shader(const Shader&) = delete;
			Shader& operator =(const Shader&) = delete;
			Shader& operator=(Shader&& other) noexcept {
				this->renderer_id = std::move(other.renderer_id);
				this->type = std::move(other.type);
				return *this;
			}


			~Shader();

			uint32_t GetRendererID() const {
				return renderer_id;
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

		// Shaders must be added on creation, cannot be added later
		class ShaderProgram {

			uint32_t renderer_id = 0;

			std::unordered_map<std::string, Uniform> uniforms;

			OpenGL::VertexLayout layout;

		public:
			ShaderProgram(std::initializer_list<const Shader*> shaders, const VertexLayout& layout);

			ShaderProgram(ShaderProgram&& other) noexcept :
				renderer_id(std::move(other.renderer_id)), uniforms(std::move(other.uniforms)) {
				other.renderer_id = 0;
			}

			ShaderProgram(const ShaderProgram&) = delete;
			ShaderProgram& operator =(const ShaderProgram&) = delete;
			ShaderProgram& operator=(ShaderProgram&& other) noexcept {
				this->renderer_id = std::move(other.renderer_id);
				this->uniforms = std::move(other.uniforms);
				other.renderer_id = 0;
				return *this;
			}

			const VertexLayout& GetLayout() const { return layout; }

			~ShaderProgram();

			void BindProgram();

			void SetUniform1F(std::string_view name, float value);
			void SetUniform2F(std::string_view name, float value_1, float value_2);
			void SetUniform3F(std::string_view name, float value_1, float value_2, float value_3);
			void SetUniform4F(std::string_view name, float value_1, float value_2, float value_3, float value4);
			void SetUniform1I(std::string_view name, int32_t value);
			void SetUniform2I(std::string_view name, int32_t value_1, int32_t value_2);
			void SetUniform3I(std::string_view name, int32_t value_1, int32_t value_2, int32_t value_3);
			void SetUniform4I(std::string_view name, int32_t value_1, int32_t value_2, int32_t value_3, int32_t value4);
			void SetUniform1UI(std::string_view name, uint32_t value);
			void SetUniform2UI(std::string_view name, uint32_t value_1, uint32_t value_2);
			void SetUniform4UI(std::string_view name, uint32_t value_1, uint32_t value_2, uint32_t value_3);
			void SetUniform4UI(std::string_view name, uint32_t value_1, uint32_t value_2, uint32_t value_3, uint32_t value4);

			// count should be 1 unless it is an array, then count is array length.
			void SetUniform1FV(std::string_view name, int32_t count, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform2FV(std::string_view name, int32_t count, float const* value);
			// count should be 1 unless it is an array, then count is array length.	
			void SetUniform3FV(std::string_view name, int32_t count, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform4FV(std::string_view name, int32_t count, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform1IV(std::string_view name, int32_t count, int32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform2IV(std::string_view name, int32_t count, int32_t const* value);
			// count should be 1 unless it is an array, then count is array length.	
			void SetUniform3IV(std::string_view name, int32_t count, int32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform4IV(std::string_view name, int32_t count, int32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value);
			// count should be 1 unless it is an array, then count is array length.	
			void SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.	
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value);

			void SetUniform1D(std::string_view name, double value);
			void SetUniform2D(std::string_view name, double value_1, double value_2);
			void SetUniform3D(std::string_view name, double value_1, double value_2, double value_3);
			void SetUniform4D(std::string_view name, double value_1, double value_2, double value_3, double value4);

			// count should be 1 unless it is an array, then count is array length.
			void SetUniform1DV(std::string_view name, int32_t count, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform2DV(std::string_view name, int32_t count, double const* value);
			// count should be 1 unless it is an array, then count is array length.	
			void SetUniform3DV(std::string_view name, int32_t count, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniform4DV(std::string_view name, int32_t count, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			void SetUniformMatrix2DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.	
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix2x4DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4x2DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3x4DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix4x3DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix2x3DV(std::string_view name, int32_t count, bool transpose, double const* value);
			// count should be 1 unless it is an array, then count is array length.
			// transpose will decide whether the matrix is flipped when sent to the shader
			void SetUniformMatrix3x2DV(std::string_view name, int32_t count, bool transpose, double const* value);



		};
	}

}
