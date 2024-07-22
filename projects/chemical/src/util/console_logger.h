#pragma once

#include "stopwatch.h"


namespace Chemical {

	namespace Util {

		enum class Severity {
			_DEFAULT = 7,
			_SUCCESS = 10,
			_WARNING = 6,
			_ERROR = 4,
			_NONE = 0
		};

		// singleton pattern
		class ConsoleLogger {
			friend std::unique_ptr<ConsoleLogger> std::make_unique<ConsoleLogger>();

		public:

			static const std::unique_ptr<ConsoleLogger>& GetInstance();

			// Only to be called by preprocessor macros in defines.h
			void CustomPrint(std::string_view message, Severity severity, int32_t line, std::filesystem::path file, std::format_args args);

			// Only to be called by preprocessor macros in defines.h
			void Print(std::string_view message, Severity severity, int32_t line, std::filesystem::path file);

			// Returns whether an error has occurred since the last time ReadError() was called.
			bool QueryError();



		private:

			bool unqueried_error_event = false;

			static std::unique_ptr<ConsoleLogger> instance;

			ConsoleLogger();
			Stopwatch stopwatch;

			void SetConsoleTextColour(int32_t ansiColour) const;

			void RetrieveConsoleHandle();

			HANDLE h_console;

		};

	}



}


