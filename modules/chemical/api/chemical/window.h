#pragma once

#include <string_view>
#include <optional>

struct GLFWwindow;

namespace Chemical {

	using Window = GLFWwindow*;

	// Creates and binds new window
	extern std::optional<Window> CreateNewWindow(const std::string_view& title, unsigned int width, unsigned int height);

	extern void DestroyWindow(Window window);
	// Binds window
	extern void BindWindow(Window window);

	// Returns whether window should close
	extern bool WindowShouldClose(Window window);
}
