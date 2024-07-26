#pragma once

#include "rendering/renderer.h"
#include "event_system.h"
#include "gui/master.h"
#include "game.h"

namespace Chemical {
	
	class ChemicalEngine {
	public:
		ChemicalEngine();
		~ChemicalEngine();

		void Update();

		bool is_running();

		std::unique_ptr<EventSystem> event_system;
		std::unique_ptr<GLFWWrapper> glfw_wrapper;
		std::unique_ptr<Renderer3D> renderer;
		std::unique_ptr<GUI> gui;
		std::unique_ptr<Game> game;

		Scene* running_scene = nullptr;

	private:

		bool running = true;

		void WindowCloseEvent(GLFWwindow* window);
	};
}
