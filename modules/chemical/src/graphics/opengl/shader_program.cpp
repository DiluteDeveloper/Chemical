#include "pch.h"

#include <glad/glad.h>

#include "shader_program.h"


namespace Chemical {


	namespace OpenGL {
		Shader::Shader(const char* data, ShaderType type) :
			type(type), rendererID(glCreateShader((GLenum)type)) {

			glShaderSource(rendererID, 1, &data, nullptr);

			glCompileShader(rendererID);

			int success;
			glGetShaderiv(rendererID, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {

				char infoLog[1024];
				glGetShaderInfoLog(rendererID, 1024, nullptr, infoLog);

#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_CUSTOM_ERROR("Shader compilation failed. [{}] \n{}",
					(uint8_t)type, infoLog);
#endif
				throw std::exception();
			}
		}

		Shader::~Shader() {
			glDeleteShader(rendererID);
		}

		void ShaderProgram::BindProgram() {
			glUseProgram(rendererID);
		}

		ShaderProgram::~ShaderProgram() {
			glDeleteProgram(rendererID);
		}

		ShaderProgram::ShaderProgram(std::initializer_list<const Shader*> shaders, const VertexLayout& layout) :
			layout(layout) {
			rendererID = glCreateProgram();

			for (const Shader* shader : shaders) {
				glAttachShader(rendererID, shader->GetRendererID());
			}

			glLinkProgram(rendererID);

			int success;
			glGetProgramiv(rendererID, GL_LINK_STATUS, &success);
			if (success != GL_TRUE) {
				char infoLog[1024];
				glGetProgramInfoLog(rendererID, 1024, nullptr, infoLog);

#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_CUSTOM_ERROR("ShaderProgram compilation failed. \n{}", infoLog);
#endif
				throw std::exception();
			}
			glValidateProgram(rendererID);

			uniforms.clear();
			int32_t uniform_count = 0;
			glGetProgramiv(rendererID, GL_ACTIVE_UNIFORMS, &uniform_count);

			if (uniform_count > 0) {
				int32_t max_name_len = 0;
				int32_t length = 0;
				int32_t count = 0;
				uint32_t type = GL_NONE;
				glGetProgramiv(rendererID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

				auto uniform_name = std::make_unique<char[]>(max_name_len);

				for (GLint i = 0; i < uniform_count; ++i)
				{
					glGetActiveUniform(rendererID, i, max_name_len, &length, &count, &type, uniform_name.get());

					int32_t location = glGetUniformLocation(rendererID, uniform_name.get());
					uniforms[uniform_name.get()] = Uniform(location);
				}
			}
		}

		void ShaderProgram::SetUniform1F(std::string_view name, float value) {
			glProgramUniform1f(rendererID, uniforms[name.data()].location, (GLfloat)value);
		}
		void ShaderProgram::SetUniform2F(std::string_view name, float value1, float value2) {
			glProgramUniform2f(rendererID, uniforms[name.data()].location, (GLfloat)value1, (GLfloat)value2);
		}
		void ShaderProgram::SetUniform3F(std::string_view name, float value1, float value2, float value3) {
			glProgramUniform3f(rendererID, uniforms[name.data()].location, (GLfloat)value1, (GLfloat)value2, (GLfloat)value3);
		}
		void ShaderProgram::SetUniform4F(std::string_view name, float value1, float value2, float value3, float value4) {
			glProgramUniform4f(rendererID, uniforms[name.data()].location, (GLfloat)value1, (GLfloat)value2, (GLfloat)value3, (GLfloat)value4);
		}
		void ShaderProgram::SetUniform1I(std::string_view name, int32_t value) {
			glProgramUniform1i(rendererID, uniforms[name.data()].location, (GLint)value);
		}
		void ShaderProgram::SetUniform2I(std::string_view name, int32_t value1, int32_t value2) {
			glProgramUniform2i(rendererID, uniforms[name.data()].location, (GLint)value1, (GLint)value2);
		}
		void ShaderProgram::SetUniform3I(std::string_view name, int32_t value1, int32_t value2, int32_t value3) {
			glProgramUniform3i(rendererID, uniforms[name.data()].location, (GLint)value1, (GLint)value2, (GLint)value3);
		}
		void ShaderProgram::SetUniform4I(std::string_view name, int32_t value1, int32_t value2, int32_t value3, int32_t value4) {
			glProgramUniform4i(rendererID, uniforms[name.data()].location, (GLint)value1, (GLint)value2, (GLint)value3, (GLint)value4);
		}
		void ShaderProgram::SetUniform1UI(std::string_view name, uint32_t value) {
			glProgramUniform1ui(rendererID, uniforms[name.data()].location, (GLuint)value);
		}
		void ShaderProgram::SetUniform2UI(std::string_view name, uint32_t value1, uint32_t value2) {
			glProgramUniform2ui(rendererID, uniforms[name.data()].location, (GLuint)value1, (GLuint)value2);
		}
		void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3) {
			glProgramUniform3ui(rendererID, uniforms[name.data()].location, (GLuint)value1, (GLuint)value2, (GLuint)value3);
		}
		void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value1, uint32_t value2, uint32_t value3, uint32_t value4) {
			glProgramUniform4ui(rendererID, uniforms[name.data()].location, (GLuint)value1, (GLuint)value2, (GLuint)value3, (GLuint)value4);
		}

		void ShaderProgram::SetUniform1FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform1fv(rendererID, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform2FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform2fv(rendererID, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform3FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform3fv(rendererID, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform4FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform4fv(rendererID, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform1IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform1iv(rendererID, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform2IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform2iv(rendererID, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform3IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform3iv(rendererID, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform4IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform4iv(rendererID, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform1uiv(rendererID, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform2uiv(rendererID, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform3uiv(rendererID, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform4uiv(rendererID, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2x4fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4x2fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3x4fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4x3fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2x3fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3x2fv(rendererID, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}

		void ShaderProgram::SetUniform1D(std::string_view name, double value) {
			glProgramUniform1d(rendererID, uniforms[name.data()].location, (GLdouble)value);
		}
		void ShaderProgram::SetUniform2D(std::string_view name, double value1, double value2) {
			glProgramUniform2d(rendererID, uniforms[name.data()].location, (GLdouble)value1, (GLdouble)value2);
		}
		void ShaderProgram::SetUniform3D(std::string_view name, double value1, double value2, double value3) {
			glProgramUniform3d(rendererID, uniforms[name.data()].location, (GLdouble)value1, (GLdouble)value2, (GLdouble)value3);
		}
		void ShaderProgram::SetUniform4D(std::string_view name, double value1, double value2, double value3, double value4) {
			glProgramUniform4d(rendererID, uniforms[name.data()].location, (GLdouble)value1, (GLdouble)value2, (GLdouble)value3, (GLdouble)value4);
		}

		void ShaderProgram::SetUniform1DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform1dv(rendererID, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform2DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform2dv(rendererID, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform3DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform3dv(rendererID, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform4DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform4dv(rendererID, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2x4dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4x2dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3x4dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4x3dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2x3dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3x2dv(rendererID, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
	}
}
