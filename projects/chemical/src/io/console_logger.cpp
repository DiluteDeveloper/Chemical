#include "pch.h"
#include "console_logger.h"

namespace Chemical {


	bool ConsoleLogger::unqueried_error_event = false;

	Util::Stopwatch ConsoleLogger::stopwatch;

	HANDLE h_console;

	bool ConsoleLogger::QueryError() {
			
		if (unqueried_error_event) {
			unqueried_error_event = false;
			return true;
		} else
			return false;

				
	}
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

		if (severity == Severity::_ERROR)
			unqueried_error_event = true;
	}



}
