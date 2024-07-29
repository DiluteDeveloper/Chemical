#include <pch.h>
#include <GLAD/glad.h>
#include "engine.h"
#include "defines.h"

namespace Chemical {
	ChemicalEngine::ChemicalEngine() {

		ConsoleLogger::Initialize();

		dispatcher = std::make_unique<EventDispatcher>();
		
		dispatcher->subscribe(WindowCloseEvent::descriptor, std::bind(&ChemicalEngine::_OnWindowClose, this, std::placeholders::_1));
		dispatcher->subscribe(InputEvent::descriptor, std::bind(&ChemicalEngine::_OnInput, this, std::placeholders::_1));

		glfw_wrapper = std::make_unique<GLFWWrapper>(dispatcher.get());
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

		glfwSwapBuffers(glfw_wrapper->GetGLFWWindow());

		glfwPollEvents();
	}

	void ChemicalEngine::_OnWindowClose(const Event& event) {
		_running = false;
	}

	void ChemicalEngine::_OnInput(const Event& event) {
		const InputEvent& event_actual = static_cast<const InputEvent&>(event);

		if (event_actual.key == GLFW_KEY_ESCAPE && event_actual.action == GLFW_PRESS) {

			Camera& camera = renderer->GetCamera();
			if(glfwGetInputMode(event_actual.window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
				glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			camera.SetEnabled(!camera.GetEnabled());
		}
	}

	bool ChemicalEngine::is_running() {
		return _running;
	}
}