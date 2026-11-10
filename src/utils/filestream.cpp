#include <spdlog/spdlog.h>
#include <fstream>
#include <iterator>

#include "filestream.h"

namespace Chemical {


	namespace Utils {
		std::string ReadFile(const std::filesystem::path& filePath) {

			std::ifstream file(filePath);
#ifdef CHEMICAL_DEBUG
			if (!file) {
				spdlog::info("Failed to read file: {}.", filePath.string());
			}
#endif
			std::string str((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
			file.close();
			return str;
		}

		std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& filePath) {
			std::ifstream file(filePath);
#ifdef CHEMICAL_DEBUG
			if (file.fail()) {
				spdlog::info("Failed to read file line by line: {}.", filePath.string());
			}
#endif
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

}
