#pragma once

struct GLFWwindow;

#include <vendor/glm/glm.hpp>
#include <string>

#include "chemical/core/LayerStack.h"

namespace Chemical {

	namespace Core {

		struct WindowSettings { // wierd having this in layers

			bool vSync = false;
			bool fullscreen = false;

			glm::ivec2 resolution = glm::ivec2(1280, 720);

			std::string title = "Chemical";

			WindowSettings() = default;
			~WindowSettings() = default;
			WindowSettings(const WindowSettings&) = default;
			WindowSettings(WindowSettings&&) = default;
		};

		class WindowLayer : public Layer {
			friend class LayerStack;

			WindowLayer(const WindowLayer&) = delete;
			WindowLayer(WindowLayer&&) = delete;

			~WindowLayer();
			WindowLayer(const WindowSettings& settings);

			GLFWwindow* glfwWindow = nullptr;

		public:

			double GetTime() const;
			bool ShouldClose() const;

			void PollEvents() const;
			void SwapBuffers() const;
		};
	}

}