#pragma once


namespace Chemical {

	namespace Util {

		class FileStream {
		public:
			std::optional<std::string> ReadFile(const std::filesystem::path& filePath);

			std::optional<std::vector<std::string>> ReadFileLineByLine(const std::filesystem::path& filePath);

			void WriteFile(const std::filesystem::path& filePath, const std::string& value);
		};

	}


}
