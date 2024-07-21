#include "pch.h"


namespace Chemical {


	namespace Util {

		HANDLE h_console;
		Stopwatch ConsoleLogger::stopwatch;

		void ConsoleLogger::SetConsoleTextColour(int32_t ansiColour) {
			if (!SetConsoleTextAttribute(h_console, (WORD)ansiColour)) {
				std::cout << "Setting console text attribute failed. Maybe InitializeLogger() wasnt called." << std::endl;
			}
		}


		void ConsoleLogger::RetrieveConsoleHandle() {
			h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		void ConsoleLogger::InitializeLogger() {
			RetrieveConsoleHandle();

			LOGGER_CONSOLE_MESSAGE("Logger initialized.");
		}


		void ConsoleLogger::ConsoleLogCustomMessage(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));
			auto messageFormatted = std::vformat(message, args);

			SetConsoleTextColour(10);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << messageFormatted << std::endl;
		}
		void ConsoleLogger::ConsoleLogMessage(std::string_view message, int32_t line, std::filesystem::path file) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));

			SetConsoleTextColour(10);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << message << std::endl;
		}
		void ConsoleLogger::ConsoleLogCustomWarning(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));
			auto messageFormatted = std::vformat(message, args);

			SetConsoleTextColour(6);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << messageFormatted << std::endl;
		}
		void ConsoleLogger::ConsoleLogWarning(std::string_view message, int32_t line, std::filesystem::path file) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));

			SetConsoleTextColour(6);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << message << std::endl;
		}

		void ConsoleLogger::ConsoleLogCustomError(std::string_view message, int32_t line, std::filesystem::path file, std::format_args args) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));
			auto messageFormatted = std::vformat(message, args);

			SetConsoleTextColour(4);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << messageFormatted << std::endl;

		}
		void ConsoleLogger::ConsoleLogError(std::string_view message, int32_t line, std::filesystem::path file) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));

			SetConsoleTextColour(4);
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << message << std::endl;

		}
	}


}
