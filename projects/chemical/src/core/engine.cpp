#include "pch.h"

#include "engine.h"
#include "util/logger.h"

namespace Chemical {
	namespace Core {

		Engine::Engine() {

			Util::Logger::InitializeLogger();

			windowSystem = std::make_unique<WindowSystem>(1280, 720, "Chemical");

			input = std::make_unique<Input>(windowSystem->window);
			renderer = std::make_unique<Renderer>(windowSystem->window, glm::fvec3(0.5f, 0.3f, 0.2f));
		}

		Engine::~Engine() {
#ifdef CHEMICAL_DEBUG
			LOGGER_CONSOLE_MESSAGE("Engine is shutting down...");
#endif
		}

		void Engine::Update() {


			renderer->Update();

			input->PollEvents();
		}
	}

}