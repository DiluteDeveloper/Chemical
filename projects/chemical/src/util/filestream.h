#pragma once


namespace Chemical {


	namespace Util {
		std::string ReadFile(const std::filesystem::path& file_path);

		std::vector<std::string> ReadFileLineByLine(const std::filesystem::path& file_path);

		void WriteFile(const std::filesystem::path& file_path, const std::string& value);
	}


}
