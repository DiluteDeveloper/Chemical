#include "Shader.h"
#include "chemical/io/FileStream.h"
#include "chemical/io/ConsoleStream.h"

#include <iostream>
#include <vendor/glad/glad.h>
#include <vendor/glm/gtc/matrix_transform.hpp>
#include <format>

namespace Chemical {

	namespace Render {
		void ShaderCompileStatus(unsigned int shader) {
			int success;
			char infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (success != GL_TRUE) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
				IO::LogError("Chemical::Render::ShaderCompileStatus({}) shader failed to compile. ({})", shader, infoLog);
				throw std::exception();
			}
		}

		Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& uniformPath) {

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

				IO::LogError("Chemical::Render::Shader::Shader(\"{}\", \"{}\", \"{}\") program failed to compile. ({})", 
					vertexPath.generic_string(), fragmentPath, uniformPath, infoLog);

				throw std::exception();
			}

			std::vector<std::string> lines = IO::ReadFileLineByLine(uniformPath);

			for (const std::string& s : lines) {
				int location = glGetUniformLocation(ID, s.c_str());
				if (location == -1)
					IO::LogWarning("Chemical::Render::Shader::Shader(\"{}\", \"{}\", \"{}\") uniform \"{}\" was not found.", vertexPath, fragmentPath, uniformPath, s);
				else
					uniformLocations.emplace(s, location);
			}

		}

		Shader::~Shader() {
			//glDeleteProgram(ID); temporarily commented
		}

		void Shader::Bind() const {
			glUseProgram(ID);
		}

		void Shader::SetUniform4FV(const std::string& name, const glm::fvec4& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform4fv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform3FV(const std::string& name, const glm::fvec3& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform3fv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform2FV(const std::string& name, const glm::fvec2& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform2fv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform1FV(const std::string& name, float value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform1fv(location->second, 1, &value);
		}

		void Shader::SetUniform4IV(const std::string& name, const glm::ivec4& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform4iv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform3IV(const std::string& name, const glm::ivec3& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform3iv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform2IV(const std::string& name, const glm::ivec2& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform2iv(location->second, 1, &value[0]);
		}
		void Shader::SetUniform1IV(const std::string& name, int value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniform1iv(location->second, 1, &value);
		}

		void Shader::SetUniformMatrix4FV(const std::string& name, const glm::fmat4& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniformMatrix4fv(location->second, 1, GL_FALSE, &value[0][0]);
		}
		void Shader::SetUniformMatrix3FV(const std::string& name, const glm::fmat3& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniformMatrix3fv(location->second, 1, GL_FALSE, &value[0][0]);
		}
		void Shader::SetUniformMatrix2FV(const std::string& name, const glm::fmat2& value) const {
			auto location = uniformLocations.find(name);
			if (location != uniformLocations.end())
				glUniformMatrix2fv(location->second, 1, GL_FALSE, &value[0][0]);
		}
	}

}
