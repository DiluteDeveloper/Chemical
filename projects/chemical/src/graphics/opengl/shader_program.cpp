#include "pch.h"

#include <glad/glad.h>

#include "shader_program.h"


namespace Chemical {


	namespace OpenGL {
		Shader::Shader(const char* data, ShaderType type) :
			type(type), renderer_id(glCreateShader((GLenum)type)) {

			glShaderSource(renderer_id, 1, &data, nullptr);

			glCompileShader(renderer_id);

			int success;
			glGetShaderiv(renderer_id, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {

				char infoLog[1024];
				glGetShaderInfoLog(renderer_id, 1024, nullptr, infoLog);

#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_CUSTOM_ERROR("Shader compilation failed. [{}] \n{}",
					(uint8_t)type, infoLog);
#endif
				throw std::exception();
			}
		}

		Shader::~Shader() {
			glDeleteShader(renderer_id);
		}

		void ShaderProgram::BindProgram() {
			glUseProgram(renderer_id);
		}

		ShaderProgram::~ShaderProgram() {
			glDeleteProgram(renderer_id);
		}

		ShaderProgram::ShaderProgram(std::initializer_list<const Shader*> shaders, const VertexLayout& layout) :
			layout(layout) {
			renderer_id = glCreateProgram();

			for (const Shader* shader : shaders) {
				glAttachShader(renderer_id, shader->GetRendererID());
			}

			glLinkProgram(renderer_id);

			int success;
			glGetProgramiv(renderer_id, GL_LINK_STATUS, &success);
			if (success != GL_TRUE) {
				char infoLog[1024];
				glGetProgramInfoLog(renderer_id, 1024, nullptr, infoLog);

#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_CUSTOM_ERROR("ShaderProgram compilation failed. \n{}", infoLog);
#endif
				throw std::exception();
			}
			glValidateProgram(renderer_id);

			uniforms.clear();
			int32_t uniform_count = 0;
			glGetProgramiv(renderer_id, GL_ACTIVE_UNIFORMS, &uniform_count);

			if (uniform_count > 0) {
				int32_t max_name_len = 0;
				int32_t length = 0;
				int32_t count = 0;
				uint32_t type = GL_NONE;
				glGetProgramiv(renderer_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

				auto uniform_name = std::make_unique<char[]>(max_name_len);

				for (GLint i = 0; i < uniform_count; ++i)
				{
					glGetActiveUniform(renderer_id, i, max_name_len, &length, &count, &type, uniform_name.get());

					int32_t location = glGetUniformLocation(renderer_id, uniform_name.get());
					uniforms[uniform_name.get()] = Uniform(location);
				}
			}
		}

		void ShaderProgram::SetUniform1F(std::string_view name, float value) {
			glProgramUniform1f(renderer_id, uniforms[name.data()].location, (GLfloat)value);
		}
		void ShaderProgram::SetUniform2F(std::string_view name, float value_1, float value_2) {
			glProgramUniform2f(renderer_id, uniforms[name.data()].location, (GLfloat)value_1, (GLfloat)value_2);
		}
		void ShaderProgram::SetUniform3F(std::string_view name, float value_1, float value_2, float value_3) {
			glProgramUniform3f(renderer_id, uniforms[name.data()].location, (GLfloat)value_1, (GLfloat)value_2, (GLfloat)value_3);
		}
		void ShaderProgram::SetUniform4F(std::string_view name, float value_1, float value_2, float value_3, float value4) {
			glProgramUniform4f(renderer_id, uniforms[name.data()].location, (GLfloat)value_1, (GLfloat)value_2, (GLfloat)value_3, (GLfloat)value4);
		}
		void ShaderProgram::SetUniform1I(std::string_view name, int32_t value) {
			glProgramUniform1i(renderer_id, uniforms[name.data()].location, (GLint)value);
		}
		void ShaderProgram::SetUniform2I(std::string_view name, int32_t value_1, int32_t value_2) {
			glProgramUniform2i(renderer_id, uniforms[name.data()].location, (GLint)value_1, (GLint)value_2);
		}
		void ShaderProgram::SetUniform3I(std::string_view name, int32_t value_1, int32_t value_2, int32_t value_3) {
			glProgramUniform3i(renderer_id, uniforms[name.data()].location, (GLint)value_1, (GLint)value_2, (GLint)value_3);
		}
		void ShaderProgram::SetUniform4I(std::string_view name, int32_t value_1, int32_t value_2, int32_t value_3, int32_t value4) {
			glProgramUniform4i(renderer_id, uniforms[name.data()].location, (GLint)value_1, (GLint)value_2, (GLint)value_3, (GLint)value4);
		}
		void ShaderProgram::SetUniform1UI(std::string_view name, uint32_t value) {
			glProgramUniform1ui(renderer_id, uniforms[name.data()].location, (GLuint)value);
		}
		void ShaderProgram::SetUniform2UI(std::string_view name, uint32_t value_1, uint32_t value_2) {
			glProgramUniform2ui(renderer_id, uniforms[name.data()].location, (GLuint)value_1, (GLuint)value_2);
		}
		void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value_1, uint32_t value_2, uint32_t value_3) {
			glProgramUniform3ui(renderer_id, uniforms[name.data()].location, (GLuint)value_1, (GLuint)value_2, (GLuint)value_3);
		}
		void ShaderProgram::SetUniform4UI(std::string_view name, uint32_t value_1, uint32_t value_2, uint32_t value_3, uint32_t value4) {
			glProgramUniform4ui(renderer_id, uniforms[name.data()].location, (GLuint)value_1, (GLuint)value_2, (GLuint)value_3, (GLuint)value4);
		}

		void ShaderProgram::SetUniform1FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform1fv(renderer_id, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform2FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform2fv(renderer_id, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform3FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform3fv(renderer_id, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform4FV(std::string_view name, int32_t count, float const* value) {

			glProgramUniform4fv(renderer_id, uniforms[name.data()].location, count, (GLfloat*)value);
		}
		void ShaderProgram::SetUniform1IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform1iv(renderer_id, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform2IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform2iv(renderer_id, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform3IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform3iv(renderer_id, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform4IV(std::string_view name, int32_t count, int32_t const* value) {

			glProgramUniform4iv(renderer_id, uniforms[name.data()].location, count, (GLint*)value);
		}
		void ShaderProgram::SetUniform1UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform1uiv(renderer_id, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform2UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform2uiv(renderer_id, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform3UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform3uiv(renderer_id, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniform4UIV(std::string_view name, int32_t count, uint32_t const* value) {

			glProgramUniform4uiv(renderer_id, uniforms[name.data()].location, count, (GLuint*)value);
		}
		void ShaderProgram::SetUniformMatrix2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix2x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2x4fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4x2fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3x4FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3x4fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix4x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix4x3fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix2x3FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix2x3fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}
		void ShaderProgram::SetUniformMatrix3x2FV(std::string_view name, int32_t count, bool transpose, float const* value) {

			glProgramUniformMatrix3x2fv(renderer_id, uniforms[name.data()].location, count, transpose, (GLfloat*)value);
		}

		void ShaderProgram::SetUniform1D(std::string_view name, double value) {
			glProgramUniform1d(renderer_id, uniforms[name.data()].location, (GLdouble)value);
		}
		void ShaderProgram::SetUniform2D(std::string_view name, double value_1, double value_2) {
			glProgramUniform2d(renderer_id, uniforms[name.data()].location, (GLdouble)value_1, (GLdouble)value_2);
		}
		void ShaderProgram::SetUniform3D(std::string_view name, double value_1, double value_2, double value_3) {
			glProgramUniform3d(renderer_id, uniforms[name.data()].location, (GLdouble)value_1, (GLdouble)value_2, (GLdouble)value_3);
		}
		void ShaderProgram::SetUniform4D(std::string_view name, double value_1, double value_2, double value_3, double value4) {
			glProgramUniform4d(renderer_id, uniforms[name.data()].location, (GLdouble)value_1, (GLdouble)value_2, (GLdouble)value_3, (GLdouble)value4);
		}

		void ShaderProgram::SetUniform1DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform1dv(renderer_id, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform2DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform2dv(renderer_id, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform3DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform3dv(renderer_id, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniform4DV(std::string_view name, int32_t count, double const* value) {

			glProgramUniform4dv(renderer_id, uniforms[name.data()].location, count, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2x4dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4x2dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3x4DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3x4dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix4x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix4x3dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix2x3DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix2x3dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
		void ShaderProgram::SetUniformMatrix3x2DV(std::string_view name, int32_t count, bool transpose, double const* value) {

			glProgramUniformMatrix3x2dv(renderer_id, uniforms[name.data()].location, count, transpose, (GLdouble*)value);
		}
	}
}
