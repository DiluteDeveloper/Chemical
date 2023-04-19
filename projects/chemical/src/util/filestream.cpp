#include "pch.h"
#include "filestream.h"

namespace Util {
	std::string ReadFile(const std::filesystem::path& filePath) {

		std::ifstream file(filePath);
		if (!file) {
			LOGGER_CONSOLE_CUSTOM_ERROR("Failed to read file: {}.", filePath.string());
		}
		std::string str((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
		file.close();
		return str;
	}

	std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& filePath) {
		std::ifstream file(filePath);
		if (file.fail()) {
			LOGGER_CONSOLE_CUSTOM_ERROR("Failed to read file line by line: {}.", filePath.string());
		}
		std::vector<std::string> lines;
		std::copy(std::istream_iterator<std::string>(file),
			std::istream_iterator<std::string>(),
			std::back_inserter(lines));
		return lines;

	}

	void WriteFile(const std::filesystem::path& filePath, const std::string& value) {
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
