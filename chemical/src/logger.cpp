#include "logger.h"

static HANDLE hConsole;
Stopwatch Logger::m_stopWatch;

void Logger::SetConsoleTextColour(int32_t ansiColour) {
	if (!SetConsoleTextAttribute(hConsole, (WORD)ansiColour)) {
		std::cout << "Setting console text attribute failed. Maybe InitializeLogger() wasnt called." << std::endl;
	}
}


void Logger::RetrieveConsoleHandle() {
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Logger::InitializeLogger() {
	RetrieveConsoleHandle();

	LOGGER_CONSOLE_MESSAGE("Logger initialized.");
}