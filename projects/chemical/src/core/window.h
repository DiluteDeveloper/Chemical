#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Chemical {
	namespace Core {
		class Window {
		public:

			// Requires Chemical::Util::Logger
			static void InitializeWindow();

			static bool WindowShouldClose();
			static void SwapBuffers();

			static GLFWwindow* glfw_window;
			static const std::string glsl_version;
			static const glm::ivec2 window_size;
			static const std::string window_name;

		};
	}
}
