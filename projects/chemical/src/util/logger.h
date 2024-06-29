#pragma once

#include "stopwatch.h"


namespace Chemical {

	namespace Util {

#ifdef CHEMICAL_DEBUG

		class Logger {

			static Stopwatch stopwatch;

			static void SetConsoleTextColour(int32_t ansi_colour);

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

#endif

#define LOGGER_CONSOLE_MESSAGE(message) Util::Logger::ConsoleLogMessage(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_MESSAGE(message, ...) Util::Logger::ConsoleLogCustomMessage(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_WARNING(message) Util::Logger::ConsoleLogWarning(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_WARNING(message, ...) Util::Logger::ConsoleLogCustomWarning(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_ERROR(message) Util::Logger::ConsoleLogError(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_ERROR(message, ...) Util::Logger::ConsoleLogCustomError(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))

	}



}


