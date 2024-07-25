#pragma once

#ifdef CHEMICAL_DEBUG

	#define DEBUG_CALL(x) x

#else

	#define DEBUG_CALL(x)

#endif

#include "io/console_logger.h"

#define CHEMICAL_PRINT(severity, message) Chemical::ConsoleLogger::Print(message, severity, __LINE__, __FILE__)

#define CHEMICAL_CUSTOM_PRINT(severity, message, ...) \
Chemical::ConsoleLogger::CustomPrint(message, severity, __LINE__, __FILE__, __VA_ARGS__)

#define QUERY_ERROR Chemical::ConsoleLogger::QueryError()

