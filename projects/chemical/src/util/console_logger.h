#pragma once

#include "stopwatch.h"


namespace Chemical {

	namespace Util {

		class ConsoleLogger {

			static Stopwatch stopwatch;

			static void SetConsoleTextColour(int32_t ansiColour);

			static void RetrieveConsoleHandle();

		public:

			static void InitializeLogger();

			static void ConsoleLogCustomMessage(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args);
			static void ConsoleLogMessage(std::string_view message, int32_t line, std::filesystem::path file);
			static void ConsoleLogCustomWarning(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args);
			static void ConsoleLogWarning(std::string_view message, int32_t line, std::filesystem::path file);
			static void ConsoleLogCustomError(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args);
			static void ConsoleLogError(std::string_view message, int32_t line, std::filesystem::path file);


		};

#define LOGGER_CONSOLE_MESSAGE(message) Util::ConsoleLogger::ConsoleLogMessage(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_MESSAGE(message, ...) Util::ConsoleLogger::ConsoleLogCustomMessage(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_WARNING(message) Util::ConsoleLogger::ConsoleLogWarning(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_WARNING(message, ...) Util::ConsoleLogger::ConsoleLogCustomWarning(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_ERROR(message) Util::ConsoleLogger::ConsoleLogError(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_ERROR(message, ...) Util::ConsoleLogger::ConsoleLogCustomError(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))

	}



}


