#include <string>
#include <fstream>
#include <algorithm>
#include <filesystem>

#include "ConsoleStream.h"

namespace Chemical {

	namespace IO {

		std::string ReadFileAppData(const std::filesystem::path& filePath) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("Chemical::Core::IO::ReadFileAppData(\"{}\") finding LOCALAPPDATA failed.", filePath);
				throw std::exception();
			}

			std::filesystem::path fullPath = std::filesystem::path(appdata) / filePath;
			free(appdata);
			std::ifstream file(fullPath);
			if (file.fail()) {
				LogError("Chemical::Core::IO::ReadFileAppData(\"{}\") filestream failed.", filePath);
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		void WriteFileAppData(const std::filesystem::path& filePath, const std::string& value) {
			char* appdata = nullptr;
			if (_dupenv_s(&appdata, nullptr, "LOCALAPPDATA") != 0 || appdata == nullptr) {
				free(appdata);
				LogError("Chemical::Core::IO::WriteFileAppData(\"{}\", \"{}\") finding LOCALAPPDATA failed.", filePath, value);
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

		std::string ReadFile(const std::filesystem::path& filePath) {

			std::ifstream file(filePath);
			if (file.fail()) {
				LogError("Chemical::Core::IO::ReadFile(\"{}\") filestream failed.", filePath);
				throw std::exception();
			}
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& filePath) {
			std::ifstream file(filePath);
			if (file.fail()) {
				LogError("Chemical::Core::IO::ReadFileLineByLine(\"{}\") filestream failed.", filePath);
				throw std::exception();
			}
			std::vector<std::string> lines;
			std::copy(std::istream_iterator<std::string>(file),
				std::istream_iterator<std::string>(),
				std::back_inserter(lines));
			return lines;

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