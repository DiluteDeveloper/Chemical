#include <pch.h>
#include <Windows.h>

#include "console_logger.h"

namespace Chemical {

	namespace Core {

		Util::Stopwatch ConsoleLogger::stopwatch;

		HANDLE h_console;

		void ConsoleLogger::SetConsoleTextColour(int32_t ansiColour) {


			assert(((void)"ConsoleLogger needs to be initialized before any other ConsoleLogger function calls.", h_console != nullptr));

			SetConsoleTextAttribute(h_console, (WORD)ansiColour);
		}


		void ConsoleLogger::Initialize() {
			h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		}

		void ConsoleLogger::Print(std::string_view message, Severity severity, int32_t line, std::filesystem::path file) {

			auto output = std::format("[{}] [Line {}] [{}]: ", stopwatch.Elapsed(), line, file.filename().string());

			SetConsoleTextColour(static_cast<int32_t>(severity));
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << message << std::endl;

		}
	}




}
