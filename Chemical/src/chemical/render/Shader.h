#pragma once

#include <string>
#include <vendor/glm/glm.hpp>
#include <unordered_map>
#include <filesystem>

namespace Chemical {

	namespace Render {
		class Shader {
		public:

			unsigned int ID = 0;
			std::unordered_map<std::string, int> uniformLocations;

			void Bind() const;

			void SetUniform4FV(const std::string& name, const glm::fvec4& value) const;
			void SetUniform3FV(const std::string& name, const glm::fvec3& value) const;
			void SetUniform2FV(const std::string& name, const glm::fvec2& value) const;
			void SetUniform1FV(const std::string& name, float value) const;

			void SetUniform4IV(const std::string& name, const glm::ivec4& value) const;
			void SetUniform3IV(const std::string& name, const glm::ivec3& value) const;
			void SetUniform2IV(const std::string& name, const glm::ivec2& value) const;
			void SetUniform1IV(const std::string& name, int value) const;

			void SetUniformMatrix4FV(const std::string& name, const glm::fmat4& value) const;
			void SetUniformMatrix3FV(const std::string& name, const glm::fmat3& value) const;
			void SetUniformMatrix2FV(const std::string& name, const glm::fmat2& value) const;

			int GetUniformLocation(const std::string& name);

			Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath, const std::filesystem::path& uniformPath);
			~Shader(); // will cause RAII issues, cant b fucked
		};
	}


}
