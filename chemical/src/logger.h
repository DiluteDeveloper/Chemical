#pragma once

#include "stopwatch.h"

class Logger {

	static Stopwatch m_stopWatch;
	
	static void SetConsoleTextColour(int32_t ansiColour);

	static void RetrieveConsoleHandle();

public:

	static void InitializeLogger();

	static void ConsoleLogCustomMessage(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));
		auto messageFormatted = std::vformat(message, args);

		SetConsoleTextColour(10);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << messageFormatted << std::endl;
	}
	static void ConsoleLogMessage(std::string_view message, int32_t line, std::filesystem::path file) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));

		SetConsoleTextColour(10);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << message << std::endl;
	}
	static void ConsoleLogCustomWarning(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));
		auto messageFormatted = std::vformat(message, args);

		SetConsoleTextColour(6);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << messageFormatted << std::endl;
	}
	static void ConsoleLogWarning(std::string_view message, int32_t line, std::filesystem::path file) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));

		SetConsoleTextColour(6);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << message << std::endl;
	}

	static void ConsoleLogCustomError(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));
		auto messageFormatted = std::vformat(message, args);

		SetConsoleTextColour(4);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << messageFormatted << std::endl;

	}
	static void ConsoleLogError(std::string_view message, int32_t line, std::filesystem::path file) {

		auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(m_stopWatch.Elapsed(), line, file.filename().string()));

		SetConsoleTextColour(4);
		std::cout << output;
		SetConsoleTextColour(15);
		std::cout << message << std::endl;

	}

};

#define LOGGER_CONSOLE_MESSAGE(message) Logger::ConsoleLogMessage(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_MESSAGE(message, ...) Logger::ConsoleLogCustomMessage(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_WARNING(message) Logger::ConsoleLogWarning(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_WARNING(message, ...) Logger::ConsoleLogCustomWarning(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))
#define LOGGER_CONSOLE_ERROR(message) Logger::ConsoleLogError(message, __LINE__, __FILE__)
#define LOGGER_CONSOLE_CUSTOM_ERROR(message, ...) Logger::ConsoleLogCustomError(message, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))