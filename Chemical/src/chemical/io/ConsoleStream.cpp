#include <format>
#include <string>
#include <iostream>
#include <filesystem>

#include <windows.h>
#include <stdio.h>

#include "chemical/core/layers/WindowLayer.h"

namespace Chemical {
	namespace IO {

		static HANDLE stdoutHandle;
		static DWORD outModeInit;

		void setupConsole(void) {
			DWORD outMode = 0;
			stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			if (stdoutHandle == INVALID_HANDLE_VALUE)
				exit(GetLastError());

			if (!GetConsoleMode(stdoutHandle, &outMode))
				exit(GetLastError());

			outModeInit = outMode;

			// Enable ANSI escape codes
			outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

			if (!SetConsoleMode(stdoutHandle, outMode))
				exit(GetLastError());

		}


		void restoreConsole() {
			// Reset colors
			printf("\x1b[0m");

			// Reset console mode
			if (!SetConsoleMode(stdoutHandle, outModeInit))
				exit(GetLastError());
		}

	}
}