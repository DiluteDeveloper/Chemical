#include "pch.h"

#include "chemical/window.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace Chemical {

	std::optional<Window> CreateNewWindow(const std::string_view& title, unsigned int width, unsigned int height) {
		spdlog::info("Creating Window");
		
		GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

		if (!window) {
			spdlog::error("GLFW window creation failed");
			return nullptr;
		}
		glfwMakeContextCurrent(window);

    spdlog::info("Initialising GLAD for window \"{0}\"", title.data());
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      spdlog::critical("GLAD initialisation failed");
      return nullptr;
    }

		return (Window)window;
	}

	void DestroyWindow(Window window) {
		glfwDestroyWindow(window);
	}

	void BindWindow(Window window) {
		glfwMakeContextCurrent(window);
	}

	bool WindowShouldClose(Window window) {
		return glfwWindowShouldClose(window);
	}
}
