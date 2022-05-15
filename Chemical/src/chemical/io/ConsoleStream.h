#pragma once

#include <format>
#include <string>
#include <iostream>

namespace Chemical {
	namespace IO {

		// Logs error to console with std::format interface, using custom colour to represent error
		template<class... Args>
		inline void LogError(const std::string& error, Args&&... args) {
			std::cout << "\x1B[91m" << std::format(error, args...) << "\033[0m\t\t" << std::endl;
		}

		// Logs error to console with std::format interface, using custom colour to represent warning
		template<class... Args>
		inline void LogWarning(const std::string& warning, Args&&... args) {
			std::cout << "\x1B[93m" << std::format(warning, args...) << "\033[0m\t\t" << std::endl;
		}

		// Logs error to console with std::format interface, using custom colour to represent message
		template<class... Args>
		inline void LogMessage(const std::string& message, Args&&... args) {
			std::cout << "\x1B[92m" << std::format(message, args...) << "\033[0m\t\t" << std::endl;
		}
	}
}
