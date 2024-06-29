#include "pch.h"
#include "filestream.h"


namespace Chemical {


	namespace Util {
		std::string ReadFile(const std::filesystem::path& file_path) {

			std::ifstream file(file_path);
#ifdef CHEMICAL_DEBUG
			if (!file) {
				LOGGER_CONSOLE_CUSTOM_ERROR("Failed to read file: {}.", file_path.string());
			}
#endif
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& file_path) {
			std::ifstream file(file_path);
#ifdef CHEMICAL_DEBUG
			if (file.fail()) {
				LOGGER_CONSOLE_CUSTOM_ERROR("Failed to read file line by line: {}.", file_path.string());
			}
#endif
			std::vector<std::string> lines;
			std::copy(std::istream_iterator<std::string>(file),
				std::istream_iterator<std::string>(),
				std::back_inserter(lines));
			return lines;

		}

		void WriteFile(const std::filesystem::path& file_path, const std::string& value) {
			if (!std::filesystem::exists(file_path)) {
				std::filesystem::create_directory(file_path.parent_path());
			}

			std::ofstream file(file_path, std::ios_base::trunc | std::ofstream::out);

			if (file.fail()) {
				file.open(file_path, std::ios_base::app | std::ofstream::out);
				file << "\n";
			}
			file << value;
			file.close();
		}
	}

}
