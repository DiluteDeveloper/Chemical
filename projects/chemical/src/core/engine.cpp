#include <pch.h>

#include "engine.h"

#include "glfw_glad/init_glfw_glad.h"

#include "rendering/renderer.h"
#include "rendering/model_importer.h"
#include "world/scene.h"

namespace Chemical {
	ChemicalEngine::ChemicalEngine() {

		ConsoleLogger::Initialize();

		glfw_wrapper = std::make_unique<GLFWWrapper>();
		if (CHEMICAL_QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with GLFW or GLAD initialization.");
		}

		event_system = std::make_unique<EventSystem>(glfw_wrapper.get());
		event_system->subscribe_window_close_event([this](GLFWwindow* window) -> void {WindowCloseEvent(window);  });

		renderer = std::make_unique<Renderer3D>(glfw_wrapper.get());
		if (CHEMICAL_QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with renderer.");
		}

		gui = std::make_unique<GUI>(this);

		game = std::make_unique<Game>(this);

	}
	ChemicalEngine::~ChemicalEngine() {
		glfwTerminate();
	}

	void ChemicalEngine::Update() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game->Update(this);

		if (running_scene != nullptr)
			renderer->RenderScene(*running_scene);

		gui->Update(this);

		glfwSwapBuffers(glfw_wrapper->window);

		glfwPollEvents();
	}

	void ChemicalEngine::WindowCloseEvent(GLFWwindow* window) {
		running = false;
	}

	bool ChemicalEngine::is_running() {
		return running;
	}
}