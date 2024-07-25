#include <pch.h>

#include "engine.h"

#include "glfw_glad/init_glfw_glad.h"

#include "rendering/renderer.h"
#include "rendering/model_importer.h"

namespace Chemical {
	ChemicalEngine::ChemicalEngine() {

		ConsoleLogger::Initialize();
		glfw_wrapper = std::make_unique<GLFWWrapper>();
		event_system = std::make_unique<EventSystem>(glfw_wrapper.get());
		event_system->subscribe_window_close_event([this](GLFWwindow* window) -> void {WindowCloseEvent(window);  });

		renderer = std::make_unique<Renderer3D>(glfw_wrapper.get());

		if (QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with GLFW or GLAD initialization.");
		}

		if (QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with renderer constructor.");
		}
		ModelImporter importer;

		auto m = importer.ImportModel("resources/models/stanford-dragon.fbx");
		if (QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred with model import.");
		}
		else {
			renderer->AddMeshToRender(m.value());
		}

	}
	ChemicalEngine::~ChemicalEngine() {
		glfwTerminate();
	}
	void ChemicalEngine::Update() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer->Render();

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