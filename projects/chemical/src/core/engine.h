#pragma once

#include "rendering/renderer.h"
#include "event_system.h"

namespace Chemical {
	
	class ChemicalEngine {
	public:
		ChemicalEngine();
		~ChemicalEngine();
		void Update();

		bool is_running();

		std::unique_ptr<EventSystem> event_system;
		std::unique_ptr<GLFWWrapper> glfw_wrapper;

	private:
		std::unique_ptr<Renderer3D> renderer;

		bool running = true;

		void WindowCloseEvent(GLFWwindow* window);
	};
}
