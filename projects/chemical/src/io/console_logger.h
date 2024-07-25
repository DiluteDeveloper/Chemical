#pragma once

#include "util/stopwatch.h"


namespace Chemical {

	enum class Severity {
		_DEFAULT = 7,
		_SUCCESS = 10,
		_WARNING = 6,
		_ERROR = 4,
		_NONE = 0
	};

	class ConsoleLogger {
	public:

		static void Initialize();

		static void Print(std::string_view message, Severity severity, int32_t line, std::filesystem::path file);

		template<class... T>
		static void CustomPrint(std::string_view message, Severity severity, int32_t line, std::filesystem::path file, T... args) {

			auto output = std::format("[{}] [Line {}] [{}]: ", stopwatch.Elapsed(), line, file.filename().string());
			auto messageFormatted = std::vformat(message, std::make_format_args(args...));

			SetConsoleTextColour(static_cast<int32_t>(severity));
			std::cout << output;
			SetConsoleTextColour(15);
			std::cout << messageFormatted << std::endl;

			if (severity == Severity::_ERROR)
				unqueried_error_event = true;
		}

		// Returns whether an error has occurred since the last time ReadError() was called.
		static bool QueryError();
	private:
		static bool unqueried_error_event;

		static Util::Stopwatch stopwatch;

		static void SetConsoleTextColour(int32_t ansiColour);

	};

}


