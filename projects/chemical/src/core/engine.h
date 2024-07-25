#pragma once

#include "rendering/renderer.h"
#include "event_system.h"

namespace Chemical {
	
	class ChemicalEngine {
	public:
		ChemicalEngine();
		~ChemicalEngine();

		void EarlyUpdate();
		// rendering calls go between
		void LateUpdate();

		bool is_running();

		std::unique_ptr<EventSystem> event_system;
		std::unique_ptr<GLFWWrapper> glfw_wrapper;
		std::unique_ptr<Renderer3D> renderer;

	private:

		bool running = true;

		void WindowCloseEvent(GLFWwindow* window);
	};
}
