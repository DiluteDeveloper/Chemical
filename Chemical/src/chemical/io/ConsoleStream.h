#pragma once

#include <filesystem>
#include <iostream>

#include "chemical/core/layers/WindowLayer.h"

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
		
		void setupConsole(void);

		
		void restoreConsole();
			
	}
}

template<>
struct std::formatter<std::filesystem::path> : std::formatter<std::string> {
	auto format(std::filesystem::path p, std::format_context& ctx) {
		return std::formatter<std::string>::format(std::format("{}", p.generic_string()), ctx);
	}
};

template<>
struct std::formatter<Chemical::Core::WindowSettings> : std::formatter<std::string> {
	auto format(Chemical::Core::WindowSettings c, std::format_context& ctx) {
		return std::formatter<std::string>::format(std::format("vSync: {} | Fullscreen: {} | Resolution: {}, {} | Title: {}", 
			c.vSync, c.fullscreen, c.resolution.x, c.resolution.y, c.title), ctx);
	}
};