#include "pch.h"

#include "window_system.h"

#include <GLFW/glfw3.h>

namespace Chemical {
	namespace Core {


			WindowSystem::WindowSystem(int width, int height, const std::string_view& title) {
				if(!glfwInit()) {
#ifdef CHEMICAL_DEBUG
					LOGGER_CONSOLE_ERROR("GLFW initialization failed.");
#endif
					throw std::exception();
				}
#ifdef CHEMICAL_DEBUG
				else
					LOGGER_CONSOLE_MESSAGE("GLFW initialized.");
#endif

				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

				window = glfwCreateWindow(width, height, title.data(), NULL, NULL);

				if (!window) {
#ifdef CHEMICAL_DEBUG
					LOGGER_CONSOLE_ERROR("GLFW window creation failed.");
#endif
					throw std::exception();
				}
#ifdef CHEMICAL_DEBUG
				else
					LOGGER_CONSOLE_MESSAGE("GLFW window created.");
#endif

				glfwSwapInterval(1);

				glfwMakeContextCurrent(window);
			}

			bool WindowSystem::shouldWindowClose() {
				return glfwWindowShouldClose(window);
			}
	}
}
