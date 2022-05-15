#include "Shader.h"
#include "io/FileStream.h"
#include "io/ConsoleStream.h"

#include <iostream>
#include <vendor/glad/glad.h>
#include <vendor/glm/gtc/matrix_transform.hpp>
#include <format>

namespace Chemical {
	void ShaderCompileStatus(unsigned int shader) {
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			IO::LogError("0x0: Chemical::ShaderCompileStatus({})\n{}", shader, infoLog);
			throw std::exception();
		}
	}

	Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) :
		vertexPath(vertexPath), fragmentPath(fragmentPath) {

		std::string vertexSource = IO::ReadFile(vertexPath);
		const char* cVertexSource = vertexSource.c_str();

		unsigned int vertexShader = 0;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &cVertexSource, NULL);


		glCompileShader(vertexShader);

		ShaderCompileStatus(vertexShader);

		// FRAGMENT

		std::string fragmentSource = IO::ReadFile(fragmentPath);
		const char* cFragmentSource = fragmentSource.c_str();

		unsigned int fragmentShader = 0;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(fragmentShader, 1, &cFragmentSource, NULL);

		glCompileShader(fragmentShader);

		ShaderCompileStatus(fragmentShader);

		ID = glCreateProgram();

		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glValidateProgram(ID);

		int success;
		char infoLog[512];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			IO::LogError("0x0: Chemical::Shader::Shader(\"{}\", \"{}\")\n{}", vertexPath.generic_string(), fragmentPath.generic_string(), infoLog);
			throw std::exception();
		}
	}

	void Shader::Bind() const {
		glUseProgram(ID);
	}

	void Shader::SetUniform4FV(const std::string& name, const glm::fvec4& value) {
		glUniform4fv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform3FV(const std::string& name, const glm::fvec3& value) {
		glUniform3fv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform2FV(const std::string& name, const glm::fvec2& value) {
		glUniform2fv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform1FV(const std::string& name, float value) {
		glUniform1fv(GetUniformLocation(name), 1, &value);
	}

	void Shader::SetUniform4IV(const std::string& name, const glm::ivec4& value) {
		glUniform4iv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform3IV(const std::string& name, const glm::ivec3& value) {
		glUniform3iv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform2IV(const std::string& name, const glm::ivec2& value) {
		glUniform2iv(GetUniformLocation(name), 1, &value[0]);
	}
	void Shader::SetUniform1IV(const std::string& name, int value) {
		glUniform1iv(GetUniformLocation(name), 1, &value);
	}

	void Shader::SetUniformMatrix4FV(const std::string& name, const glm::fmat4& value) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}
	void Shader::SetUniformMatrix3FV(const std::string& name, const glm::fmat3& value) {
		glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}
	void Shader::SetUniformMatrix2FV(const std::string& name, const glm::fmat2& value) {
		glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
	}

	int Shader::GetUniformLocation(const std::string& name) {

		{
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				return location->second;
		}
		{
			int location = glGetUniformLocation(ID, name.c_str());
			if (location == -1)
				std::cout << "OpenGL Shader " << vertexPath << " : " << fragmentPath << " getUniformLocation(" << name << ") uniform doesnt exist." << std::endl;
			else
				uniformLocations.emplace(name, location);
			return location;
		}
	}
}
