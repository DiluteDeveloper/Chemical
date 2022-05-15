#pragma once

struct GLFWwindow;

#include <vendor/glm/glm.hpp>
#include <iostream>

namespace Chemical {
	namespace Layers {
		class SettingsLayer;
	}

	namespace Core {

		struct WindowSettings {

			bool vSync = false;
			bool fullscreen = false;

			glm::ivec2 resolution = glm::ivec2(1280, 720);

			std::string title = "Chemical";

			friend class Layers::SettingsLayer;

		private:
			WindowSettings() = default;
			~WindowSettings() = default;
			WindowSettings(const WindowSettings&) = delete;
			WindowSettings(WindowSettings&&) = delete;
		};
		class Window {

			friend class Application;

			Window(const Window&) = delete;
			Window(Window&&) = delete;

			~Window();
			Window(WindowSettings& settings);

			GLFWwindow* glfwWindow = nullptr;

			WindowSettings& settings;
		};
	}

}