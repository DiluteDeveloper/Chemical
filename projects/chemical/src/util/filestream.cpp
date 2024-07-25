#include "pch.h"
#include "filestream.h"

namespace Chemical {


	namespace Util {
		std::optional<std::string> FileStream::ReadFile(const std::filesystem::path& filePath) {

			std::ifstream file(filePath);

			if (!file) { 
				CHEMICAL_CUSTOM_PRINT(Severity::_ERROR, "Failed to read file: {}.", filePath.string());
				return std::nullopt; 
			}

			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::optional<std::vector<std::string>> FileStream::ReadFileLineByLine(const std::filesystem::path& filePath) {
			std::ifstream file(filePath);

			if (!file) { 
				CHEMICAL_CUSTOM_PRINT(Severity::_ERROR, "Failed to read file: {}.", filePath.string());
				return std::nullopt; 
			}

			std::vector<std::string> lines;
			std::copy(std::istream_iterator<std::string>(file),
				std::istream_iterator<std::string>(),
				std::back_inserter(lines));
			return lines;

		}

		void FileStream::WriteFile(const std::filesystem::path& filePath, const std::string& value) {
			if (!std::filesystem::exists(filePath)) {
				std::filesystem::create_directory(filePath.parent_path());
			}

			std::ofstream file(filePath, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(filePath, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();
		}
	}

}
