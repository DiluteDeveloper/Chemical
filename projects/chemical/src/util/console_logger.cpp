#include "pch.h"
#include "console_logger.h"

namespace Chemical {


	namespace Util {

		std::unique_ptr<ConsoleLogger> ConsoleLogger::instance;

		const std::unique_ptr<ConsoleLogger>& ConsoleLogger::GetInstance() {
			if (instance == nullptr)
				instance = std::make_unique<ConsoleLogger>();
			return instance;
		}

		bool ConsoleLogger::QueryError() {
			
			if (unqueried_error_event) {
				unqueried_error_event = false;
				return true;
			} else
				return false;

				
		}

		void ConsoleLogger::SetConsoleTextColour(int32_t ansiColour) const {
			if (!SetConsoleTextAttribute(h_console, (WORD)ansiColour)) {
				CONSOLE_PRINT(Severity::_ERROR, "Setting console text attribute failed. Maybe InitializeLogger() wasnt called.");
			}
		}


		void ConsoleLogger::RetrieveConsoleHandle() {
			h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		ConsoleLogger::ConsoleLogger() {
			RetrieveConsoleHandle();
		}


		void ConsoleLogger::CustomPrint(std::string_view message, Severity severity, int32_t line, std::filesystem::path file, std::format_args args) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));
			auto messageFormatted = std::vformat(message, args);

			SetConsoleTextColour(static_cast<int32_t>(severity));
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << messageFormatted << std::endl;

			if (severity == Severity::_ERROR)
				unqueried_error_event = true;
		}
		void ConsoleLogger::Print(std::string_view message, Severity severity, int32_t line, std::filesystem::path file) {

			auto output = std::vformat("[{}] [Line {}] [{}]: ", std::make_format_args(stopwatch.Elapsed(), line, file.filename().string()));

			SetConsoleTextColour(static_cast<int32_t>(severity));
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << message << std::endl;

			if (severity == Severity::_ERROR)
				unqueried_error_event = true;
		}

	}


}
