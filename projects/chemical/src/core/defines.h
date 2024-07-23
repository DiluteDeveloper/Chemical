#pragma once

#ifdef CHEMICAL_DEBUG

	#define DEBUG_CALL(x) x

#else

	#define DEBUG_CALL(x)

#endif

#include "util/console_logger.h"

#define CONSOLE_PRINT(severity, message) Chemical::Util::ConsoleLogger::GetInstance()->Print(message, Chemical::Util::severity, __LINE__, __FILE__)

#define CONSOLE_CUSTOM_PRINT(severity, message, ...) \
Chemical::Util::ConsoleLogger::GetInstance()->CustomPrint(message, Chemical::Util::severity, __LINE__, __FILE__, std::make_format_args(__VA_ARGS__))

#define QUERY_ERROR Chemical::Util::ConsoleLogger::GetInstance()->QueryError()

