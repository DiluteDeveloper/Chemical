#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "ConsoleStream.h"

namespace Chemical {

	namespace IO {

		std::string ReadFileAppData(const std::string& filePath) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("0x0: Chemical::Core::IO::ReadFileAppData(\"{}\")", filePath);
				throw std::exception();
			}

			std::filesystem::path fullPath = std::filesystem::path(appdata) / std::filesystem::path(filePath);
			free(appdata);
			std::ifstream file(fullPath);
			if (file.fail()) {
				LogError("0x1: Chemical::Core::IO::ReadFileAppData(\"{}\")", filePath);
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::string ReadFileAppData(const std::filesystem::path& filePath) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("0x0: Chemical::Core::IO::ReadFileAppData(\"{}\")", filePath.generic_string());
				throw std::exception();
			}

			std::filesystem::path fullPath = std::filesystem::path(appdata) / filePath;
			free(appdata);
			std::ifstream file(fullPath);
			if (file.fail()) {
				LogError("0x1: Chemical::Core::IO::ReadFileAppData(\"{}\")", filePath.generic_string());
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		void WriteFileAppData(const std::string& filePath, const std::string& value) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("0x0: Chemical::Core::IO::WriteFileAppData(\"{}\", \"{}\")", filePath, value);
				throw std::exception();
			}

			std::filesystem::path fullPath = std::filesystem::path(appdata) / std::filesystem::path(filePath);
			free(appdata);
			if (!std::filesystem::exists(fullPath)) {
				std::filesystem::create_directory(fullPath.parent_path());
			}

			std::ofstream file(fullPath, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(fullPath, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();

		}

		void WriteFileAppData(const std::filesystem::path& filePath, const std::string& value) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("0x0: Chemical::Core::IO::WriteFileAppData(\"{}\", \"{}\")", filePath.generic_string(), value);
				throw std::exception();
			}

			std::filesystem::path fullPath = std::filesystem::path(appdata) / filePath;
			free(appdata);
			if (!std::filesystem::exists(fullPath)) {
				std::filesystem::create_directory(fullPath.parent_path());
			}

			std::ofstream file(fullPath, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(fullPath, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();
		}

		std::string ReadFile(const std::string& filePath) {

			std::filesystem::path fullPath = std::filesystem::path(filePath);

			std::ifstream file(fullPath);
			if (file.fail()) {
				LogError("0x0: Chemical::Core::IO::ReadFile(\"{}\")", filePath);
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::string ReadFile(const std::filesystem::path& filePath) {

			std::ifstream file(filePath);
			if (file.fail()) {
				LogError("0x0: Chemical::Core::IO::ReadFile(\"{}\")", filePath.generic_string());
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		void WriteFile(const std::string& filePath, const std::string& value) {

			std::filesystem::path fullPath = std::filesystem::path(filePath);

			if (!std::filesystem::exists(fullPath)) {
				std::filesystem::create_directory(fullPath.parent_path());
			}

			std::ofstream file(fullPath, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(fullPath, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();
		}

		void WriteFile(const std::filesystem::path& filePath, const std::string& value) {
			std::filesystem::path fullPath = std::filesystem::path(filePath);

			if (!std::filesystem::exists(fullPath)) {
				std::filesystem::create_directory(fullPath.parent_path());
			}

			std::ofstream file(fullPath, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(fullPath, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();
		}
	
	}
}