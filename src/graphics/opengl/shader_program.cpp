#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>

#include <stdexcept>

#include "shader_program.h"


namespace Chemical {


	namespace OpenGL {

		namespace {
			// program currently made current through this wrapper, avoids redundant glUseProgram calls
			uint32_t currentProgram = 0;
		}

		Shader::Shader(const char* data, ShaderType type) :
			rendererID(glCreateShader((GLenum)type)), type(type) {

			glShaderSource(rendererID, 1, &data, nullptr);

			glCompileShader(rendererID);

            spdlog::info("Hello!");
			int success = 0;
			glGetShaderiv(rendererID, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {

				char infoLog[1024];
				glGetShaderInfoLog(rendererID, 1024, nullptr, infoLog);

                spdlog::info("Hello! failed");
				spdlog::error("Shader compilation failed. [{:#x}] \n{}", (uint32_t)type, infoLog);
				// the destructor won't run when the constructor throws
				glDeleteShader(rendererID);
				rendererID = 0;
				throw std::runtime_error(std::string("Shader compilation failed:\n") + infoLog);
			}
		}

		void Shader::Release() {
			if (rendererID != 0) {
				glDeleteShader(rendererID);
				rendererID = 0;
			}
		}
		Shader::~Shader() {
			Release();
		}

		void ShaderProgram::BindProgram() {
			glUseProgram(rendererID);
			currentProgram = rendererID;
		}
		void ShaderProgram::Use() {
			if (currentProgram != rendererID) {
				glUseProgram(rendererID);
				currentProgram = rendererID;
			}
		}

		void ShaderProgram::Release() {
			if (rendererID != 0) {
				if (currentProgram == rendererID)
					currentProgram = 0;
				glDeleteProgram(rendererID);
				rendererID = 0;
			}
		}
		ShaderProgram::~ShaderProgram() {
			Release();
		}

		ShaderProgram::ShaderProgram(std::initializer_list<const Shader*> shaders, const VertexLayout& layout) :
			layout(layout) {
			rendererID = glCreateProgram();

			for (const Shader* shader : shaders) {
				glAttachShader(rendererID, shader->GetRendererID());
			}

			glLinkProgram(rendererID);

			int success = 0;
			glGetProgramiv(rendererID, GL_LINK_STATUS, &success);

			if (success != GL_TRUE) {
				char infoLog[1024];
				glGetProgramInfoLog(rendererID, 1024, nullptr, infoLog);

#ifdef CHEMICAL_DEBUG
				spdlog::error("ShaderProgram compilation failed. \n{}", infoLog);
#endif
				glDeleteProgram(rendererID);
				rendererID = 0;
				throw std::runtime_error(std::string("ShaderProgram link failed:\n") + infoLog);
			}

			for (const Shader* shader : shaders) {
				glDetachShader(rendererID, shader->GetRendererID());
			}
			// glValidateProgram removed: it depends on current GL state and reports spurious failures before drawing.
		}

		int32_t ShaderProgram::GetLocation(std::string_view name) {
			std::string key(name); // string_view isn't guaranteed null terminated
			auto it = uniforms.find(key);
			if (it != uniforms.end())
				return it->second.location;

			const int32_t location = glGetUniformLocation(rendererID, key.c_str());
#ifdef CHEMICAL_DEBUG
			if (location < 0)
				spdlog::warn("Uniform '{}' not found (or optimized out).", key);
#endif
			uniforms.emplace(std::move(key), Uniform(location));
			return location;
		}

		void ShaderProgram::SetUniformBlockBinding(std::string_view blockName, uint32_t bindingIndex) {
			const std::string key(blockName);
			const GLuint blockIndex = glGetUniformBlockIndex(rendererID, key.c_str());
#ifdef CHEMICAL_DEBUG
			if (blockIndex == GL_INVALID_INDEX)
				spdlog::warn("Uniform block '{}' not found.", key);
#endif
			if (blockIndex != GL_INVALID_INDEX)
				glUniformBlockBinding(rendererID, blockIndex, bindingIndex);
		}

		void ShaderProgram::SetUniform1F(std::string_view name, float value) {
			Use();
			glUniform1f(GetLocation(name), value);
		}
		void ShaderProgram::SetUniform2F(std::string_view name, float value1, float value2) {
			Use();
			glUniform2f(GetLocation(name), value1, value2);
		}
		void ShaderProgram::SetUniform3F(std::string_view name, float value1, float value2, float value3) {
			Use();
			glUniform3f(GetLocation(name), value1, value2, value3);
		}
		void ShaderProgram::SetUniform4F(std::string_view name, float value1, float value2, float value3, float value4) {
			Use();
			glUniform4f(GetLocation(name), value1, value2, value3, value4);
		}
		void ShaderProgram::SetUniform1I(std::string_view name, int32_t value) {
			Use();
			glUniform1i(GetLocation(name), value);
		}
		void ShaderProgram::SetUniform2I(std::string_view name, int32_t value1, int32_t value2) {
			Use();
			glUniform2i(GetLocation(name), value1, value2);
		}
		void ShaderProgram::SetUniform3I(std::string_view name, int32_t value1, int32_t value2, int32_t value3) {
			Use();
			glUniform3i(GetLocation(name), value1, value2, value3);
		}
		void ShaderProgram::SetUniform4I(std::string_view name, int32_t value1, int32_t value2, int32_t value3, int32_t value4) {
			Use();
			glUniform4i(GetLocation(name), value1, value2, value3, value4);
		}
		void ShaderProgram::SetUniform1UI(std::string_view name, uint32_t value) {
			Use();
			glUniform1ui(GetLocation(name), value);
		}
		void ShaderProgram::SetUniform2UI(std::string_view name, uint32_t value1, uint32_t value2) {
			Use();
			glUniform2ui(GetLocation(name), value1, value2);
		}
		void ShaderProgram::SetUniform3UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3) {
			Use();
			glUniform3ui(GetLocation(name), value1, value2, value3);
		}
		void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4) {
			Use();
			glUniform4ui(GetLocation(name), value1, value2, value3, value4);
		}
		void ShaderProgram::SetUniform1FV(std::string_view name, int32_t count, float const* value) {
			Use();
			glUniform1fv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform2FV(std::string_view name, int32_t count, float const* value) {
			Use();
			glUniform2fv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform3FV(std::string_view name, int32_t count, float const* value) {
			Use();
			glUniform3fv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform4FV(std::string_view name, int32_t count, float const* value) {
			Use();
			glUniform4fv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform1IV(std::string_view name, int32_t count, int32_t const* value) {
			Use();
			glUniform1iv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform2IV(std::string_view name, int32_t count, int32_t const* value) {
			Use();
			glUniform2iv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform3IV(std::string_view name, int32_t count, int32_t const* value) {
			Use();
			glUniform3iv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform4IV(std::string_view name, int32_t count, int32_t const* value) {
			Use();
			glUniform4iv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value) {
			Use();
			glUniform1uiv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value) {
			Use();
			glUniform2uiv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value) {
			Use();
			glUniform3uiv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value) {
			Use();
			glUniform4uiv(GetLocation(name), count, value);
		}
		void ShaderProgram::SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix2fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix3fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix4fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix2x4fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix4x2fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix3x4fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix4x3fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix2x3fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
		void ShaderProgram::SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
			Use();
			glUniformMatrix3x2fv(GetLocation(name), count, transpose ? GL_TRUE : GL_FALSE, value);
		}
	}
}
