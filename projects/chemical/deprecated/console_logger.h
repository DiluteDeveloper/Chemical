#pragma once

#include "util/stopwatch.h"

#define CHEMICAL_PRINT(severity, message) Chemical::Core::ConsoleLogger::Print(message, severity, __LINE__, __FILE__)

#define CHEMICAL_CUSTOM_PRINT(severity, message, ...) \
Chemical::Core::ConsoleLogger::CustomPrint(message, severity, __LINE__, __FILE__, __VA_ARGS__)

namespace Chemical {

	namespace Core {

		enum class Severity {
			_DEFAULT = 7,
			_SUCCESS = 10,
			_WARNING = 6,
			_ERROR = 4,
			_NONE = 0
		};

		class ConsoleLogger {
		public:

			static void Initialize();

			static void Print(std::string_view message, Severity severity, int32_t line, std::filesystem::path file);

			template<class... T>
			static void CustomPrint(std::string_view message, Severity severity, int32_t line, std::filesystem::path file, T... args) {

				auto output = std::format("[{}] [Line {}] [{}]: ", stopwatch.Elapsed(), line, file.filename().string());
				auto messageFormatted = std::vformat(message, std::make_format_args(args...));

				SetConsoleTextColour(static_cast<int32_t>(severity));
				std::cout << output;
				SetConsoleTextColour(15);
				std::cout << messageFormatted << std::endl;

			}

		private:

			static Util::Stopwatch stopwatch;

			static void SetConsoleTextColour(int32_t ansiColour);

		};
	}


}


