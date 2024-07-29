#pragma once

#ifdef CHEMICAL_DEBUG

	#define CHEMICAL_DEBUG_CALL(x) x

#else

	#define CHEMICAL_DEBUG_CALL(x)

#endif

/*#include "console_logger.h"
//#define CHEMICAL_QUERY_ERROR Chemical::ConsoleLogger::QueryError()

namespace Chemical {

	class Debug {
	public:

		using ErrorType = std::string;

		enum class Severity {
			_DEFAULT = 7,
			_SUCCESS = 10,
			_WARNING = 6,
			_ERROR = 4,
			_NONE = 0
		};
		static void PrintUncaughtErrorTypes() {
			for (const auto& [key, value] : m_unhandled_error_types) {

			}
		}

		static bool TryCatchErrorType(ErrorType type) {
			bool error = m_unhandled_error_types[type];
			m_unhandled_error_types[type] = false;
			return error;

		}
		static void ThrowErrorType(ErrorType type) {
			m_unhandled_error_types[type] = true;
		}
	private:
		static std::vector<ErrorType> m_unhandled_error_types;
	};
}
inline bool error = false;
inline bool 

*/