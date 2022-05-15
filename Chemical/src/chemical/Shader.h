#pragma once

#include <string>
#include <vendor/glm/glm.hpp>
#include <unordered_map>
#include <filesystem>

class AssetImporter;

namespace Chemical {
	class Shader {
	public:

		unsigned int ID = 0;
		const std::filesystem::path vertexPath = "";
		const std::filesystem::path fragmentPath = "";
		std::unordered_map<std::string, int> uniformLocations;

		void Bind() const;

		void SetUniform4FV(const std::string& name, const glm::fvec4& value);
		void SetUniform3FV(const std::string& name, const glm::fvec3& value);
		void SetUniform2FV(const std::string& name, const glm::fvec2& value);
		void SetUniform1FV(const std::string& name, float value);

		void SetUniform4IV(const std::string& name, const glm::ivec4& value);
		void SetUniform3IV(const std::string& name, const glm::ivec3& value);
		void SetUniform2IV(const std::string& name, const glm::ivec2& value);
		void SetUniform1IV(const std::string& name, int value);

		void SetUniformMatrix4FV(const std::string& name, const glm::fmat4& value);
		void SetUniformMatrix3FV(const std::string& name, const glm::fmat3& value);
		void SetUniformMatrix2FV(const std::string& name, const glm::fmat2& value);

		int GetUniformLocation(const std::string& name);

		Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	};

}
