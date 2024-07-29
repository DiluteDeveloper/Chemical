#include <pch.h>
#include <GLAD/glad.h>
#include "engine.h"
#include "defines.h"

namespace Chemical {
	ChemicalEngine::ChemicalEngine() {

		ConsoleLogger::Initialize();

		// should error out as not constructing as shared_ptr
		dispatcher = std::make_unique<EventDispatcher>();

		glfw_wrapper = std::make_unique<GLFWWrapper>(dispatcher);
		if (CHEMICAL_QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with GLFW or GLAD initialization.");
		}

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