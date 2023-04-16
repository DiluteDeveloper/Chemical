#include "pch.h"

#include <glad/glad.h>

#include "shader_program.h"
#include "util/logger.h"

namespace OpenGL {
	Shader::Shader(const char* data, ShaderType type) : 
		m_type(type), m_rendererID(glCreateShader((GLenum)type)) {

		glShaderSource(m_rendererID, 1, &data, nullptr);

		glCompileShader(m_rendererID);

		int success;
		glGetShaderiv(m_rendererID, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE) {

			char infoLog[512];
			glGetShaderInfoLog(m_rendererID, sizeof(infoLog), nullptr, infoLog);

			LOGGER_CONSOLE_CUSTOM_ERROR("Shader compilation failed. [{}] \n{}",
				(uint8_t)type, infoLog);
			throw std::exception();
		}
	}

	Shader::~Shader() {
		glDeleteShader(m_rendererID);
	}


	void ShaderProgram::Reload() {
		glDeleteProgram(m_rendererID);
		// maye and create, dont know if resetting is an option
		m_rendererID = glCreateProgram();

		for (const auto&[type, shader_ptr] : m_shaders) {
			glAttachShader(m_rendererID, shader_ptr->GetRendererID());
		}

		glLinkProgram(m_rendererID);

		glValidateProgram(m_rendererID);

		int success;
		glGetProgramiv(m_rendererID, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(m_rendererID, sizeof(infoLog), nullptr, infoLog);

			LOGGER_CONSOLE_CUSTOM_ERROR("ShaderProgram compilation failed. \n{}", infoLog);
			throw std::exception();
		}

		m_uniforms.clear();
		int32_t uniform_count = 0;
		glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORMS, &uniform_count);

		if (uniform_count > 0) {
			int32_t max_name_len = 0;
			int32_t length = 0;
			int32_t count = 0;
			uint32_t type = GL_NONE;
			glGetProgramiv(m_rendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

			auto uniform_name = std::make_unique<char[]>(max_name_len);

			for (GLint i = 0; i < uniform_count; ++i)
			{
				glGetActiveUniform(m_rendererID, i, max_name_len, &length, &count, &type, uniform_name.get());

				int32_t location = glGetUniformLocation(m_rendererID, uniform_name.get());
				m_uniforms[uniform_name.get()] = Uniform(location, count);
			}
		}
	}

	void ShaderProgram::AddShader(std::shared_ptr<Shader> shader) {
		if (m_shaders.contains(shader->GetShaderType()))
			LOGGER_CONSOLE_CUSTOM_WARNING("ShaderProgram already contains shader type {}. AddShader cancelled.", (uint8_t)shader->GetShaderType());
		m_shaders[shader->GetShaderType()] = shader;
	}

	void ShaderProgram::BindProgram() {
		glUseProgram(m_rendererID);
	}

	void ShaderProgram::RemoveShader(ShaderType type) {
		if (m_shaders.contains(type)) {
			m_shaders.erase(type);
		}


	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_rendererID);
	}

	void ShaderProgram::SetUniform1F(std::string_view name, float value) {
		glProgramUniform1f(m_rendererID, m_uniforms[name.data()].m_location, (GLfloat)value);
	}
	void ShaderProgram::SetUniform2F(std::string_view name, float value1, float value2) {
		glProgramUniform2f(m_rendererID, m_uniforms[name.data()].m_location, (GLfloat)value1, (GLfloat)value2);
	}
	void ShaderProgram::SetUniform3F(std::string_view name, float value1, float value2, float value3) {
		glProgramUniform3f(m_rendererID, m_uniforms[name.data()].m_location, (GLfloat)value1, (GLfloat)value2, (GLfloat)value3);
	}
	void ShaderProgram::SetUniform4F(std::string_view name, float value1, float value2, float value3, float value4) {
		glProgramUniform4f(m_rendererID, m_uniforms[name.data()].m_location, (GLfloat)value1, (GLfloat)value2, (GLfloat)value3, (GLfloat)value4);
	}
	void ShaderProgram::SetUniform1I(std::string_view name, int32_t value) {
		glProgramUniform1i(m_rendererID, m_uniforms[name.data()].m_location, (GLint)value);
	}
	void ShaderProgram::SetUniform2I(std::string_view name, int32_t value1, int32_t value2) {
		glProgramUniform2i(m_rendererID, m_uniforms[name.data()].m_location, (GLint)value1, (GLint)value2);
	}
	void ShaderProgram::SetUniform3I(std::string_view name, int32_t value1, int32_t value2, int32_t value3) {
		glProgramUniform3i(m_rendererID, m_uniforms[name.data()].m_location, (GLint)value1, (GLint)value2, (GLint)value3);
	}
	void ShaderProgram::SetUniform4I(std::string_view name, int32_t value1, int32_t value2, int32_t value3, int32_t value4) {
		glProgramUniform4i(m_rendererID, m_uniforms[name.data()].m_location, (GLint)value1, (GLint)value2, (GLint)value3, (GLint)value4);
	}
	void ShaderProgram::SetUniform1UI(std::string_view name, uint32_t value) {
		glProgramUniform1ui(m_rendererID, m_uniforms[name.data()].m_location, (GLuint)value);
	}
	void ShaderProgram::SetUniform2UI(std::string_view name, uint32_t value1, uint32_t value2) {
		glProgramUniform2ui(m_rendererID, m_uniforms[name.data()].m_location, (GLuint)value1, (GLuint)value2);
	}
	void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3) {
		glProgramUniform3ui(m_rendererID, m_uniforms[name.data()].m_location, (GLuint)value1, (GLuint)value2, (GLuint)value3);
	}
	void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4) {
		glProgramUniform4ui(m_rendererID, m_uniforms[name.data()].m_location, (GLuint)value1, (GLuint)value2, (GLuint)value3, (GLuint)value4);
	}

	void ShaderProgram::SetUniform1FV(std::string_view name, int32_t count, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform1fv(m_rendererID, uniform.m_location, uniform.m_count, (GLfloat*)value);
	}
	void ShaderProgram::SetUniform2FV(std::string_view name, int32_t count, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform2fv(m_rendererID, uniform.m_location, uniform.m_count, (GLfloat*)value);
	}
	void ShaderProgram::SetUniform3FV(std::string_view name, int32_t count, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform3fv(m_rendererID, uniform.m_location, uniform.m_count, (GLfloat*)value);
	}
	void ShaderProgram::SetUniform4FV(std::string_view name, int32_t count, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform4fv(m_rendererID, uniform.m_location, uniform.m_count, (GLfloat*)value);
	}
	void ShaderProgram::SetUniform1IV(std::string_view name, int32_t count, int32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform1iv(m_rendererID, uniform.m_location, uniform.m_count, (GLint*)value);
	}
	void ShaderProgram::SetUniform2IV(std::string_view name, int32_t count, int32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform2iv(m_rendererID, uniform.m_location, uniform.m_count, (GLint*)value);
	}
	void ShaderProgram::SetUniform3IV(std::string_view name, int32_t count, int32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform3iv(m_rendererID, uniform.m_location, uniform.m_count, (GLint*)value);
	}
	void ShaderProgram::SetUniform4IV(std::string_view name, int32_t count, int32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform4iv(m_rendererID, uniform.m_location, uniform.m_count, (GLint*)value);
	}
	void ShaderProgram::SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform1uiv(m_rendererID, uniform.m_location, uniform.m_count, (GLuint*)value);
	}
	void ShaderProgram::SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform2uiv(m_rendererID, uniform.m_location, uniform.m_count, (GLuint*)value);
	}
	void ShaderProgram::SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform3uiv(m_rendererID, uniform.m_location, uniform.m_count, (GLuint*)value);
	}
	void ShaderProgram::SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform4uiv(m_rendererID, uniform.m_location, uniform.m_count, (GLuint*)value);
	}
	void ShaderProgram::SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2x4fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4x2fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3x4fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4x3fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2x3fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}
	void ShaderProgram::SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3x2fv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLfloat*)value);
	}

	void ShaderProgram::SetUniform1D(std::string_view name, double value) {
		glProgramUniform1d(m_rendererID, m_uniforms[name.data()].m_location, (GLdouble)value);
	}
	void ShaderProgram::SetUniform2D(std::string_view name, double value1, double value2) {
		glProgramUniform2d(m_rendererID, m_uniforms[name.data()].m_location, (GLdouble)value1, (GLdouble)value2);
	}
	void ShaderProgram::SetUniform3D(std::string_view name, double value1, double value2, double value3) {
		glProgramUniform3d(m_rendererID, m_uniforms[name.data()].m_location, (GLdouble)value1, (GLdouble)value2, (GLdouble)value3);
	}
	void ShaderProgram::SetUniform4D(std::string_view name, double value1, double value2, double value3, double value4) {
		glProgramUniform4d(m_rendererID, m_uniforms[name.data()].m_location, (GLdouble)value1, (GLdouble)value2, (GLdouble)value3, (GLdouble)value4);
	}

	void ShaderProgram::SetUniform1DV(std::string_view name, int32_t count, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform1dv(m_rendererID, uniform.m_location, uniform.m_count, (GLdouble*)value);
	}
	void ShaderProgram::SetUniform2DV(std::string_view name, int32_t count, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform2dv(m_rendererID, uniform.m_location, uniform.m_count, (GLdouble*)value);
	}
	void ShaderProgram::SetUniform3DV(std::string_view name, int32_t count, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform3dv(m_rendererID, uniform.m_location, uniform.m_count, (GLdouble*)value);
	}
	void ShaderProgram::SetUniform4DV(std::string_view name, int32_t count, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniform4dv(m_rendererID, uniform.m_location, uniform.m_count, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix2DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix3DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix4DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix2x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2x4dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix4x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4x2dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix3x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3x4dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix4x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix4x3dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix2x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix2x3dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
	void ShaderProgram::SetUniformMatrix3x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {
		const Uniform& uniform = m_uniforms[name.data()];
		glProgramUniformMatrix3x2dv(m_rendererID, uniform.m_location, uniform.m_count, transpose, (GLdouble*)value);
	}
}