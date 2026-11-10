#pragma once

#include <filesystem>
#include <vector>

namespace Chemical {

	namespace Utils {
		std::string ReadFile(const std::filesystem::path& filePath);

		std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& filePath);

		void WriteFile(const std::filesystem::path& filePath, const std::string& value);
	}


}
