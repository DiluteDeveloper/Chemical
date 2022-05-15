#pragma once

#include <string>
#include <filesystem>

namespace Chemical {

	namespace IO {

		// Returns %LOCALAPPDATA%/filePath entire file string, will throw if doesnt exist
		//std::string ReadFileAppData(const std::string& filePath);
		// Returns %LOCALAPPDATA%/filePath entire file string, will throw if doesnt exist
		std::string ReadFileAppData(const std::filesystem::path& filePath);

		// Replace %LOCALAPPDATA%/filePath file with value, will create file and directory if doesnt exist
		//void WriteFileAppData(const std::string& filePath, const std::string& value);
		// Replace %LOCALAPPDATA%/filePath file with value, will create file and directory if doesnt exist
		void WriteFileAppData(const std::filesystem::path& filePath, const std::string& value);

		// Returns $(ProjectDir)filePath entire file string
		//std::string ReadFile(const std::string& filePath);
		// Returns $(ProjectDir)filePath entire file string
		std::string ReadFile(const std::filesystem::path& filePath);

		// Replace $(ProjectDir)filePath file with value, will create file and directory if doesnt exist
		//void WriteFile(const std::string& filePath, const std::string& value);
		// Replace $(ProjectDir)filePath file with value, will create file and directory if doesnt exist
		void WriteFile(const std::filesystem::path& filePath, const std::string& value);
	}
}
