#include "pch.h"

#include "Engine.h"
#include "util/Logger.h"

#include "rendering/Vertex3D.h"

namespace Chemical {
	namespace Core {

		void multiply(int a, int b) {
			LOGGER_CONSOLE_CUSTOM_MESSAGE("multiply result is {}", (a * b));
		}

		void add(int a, int b) {
			LOGGER_CONSOLE_CUSTOM_MESSAGE("multiply result is {}", (a + b));
		}

		void subtract(int a, int b) {
			LOGGER_CONSOLE_CUSTOM_MESSAGE("multiply result is {}", (a - b));
		}


		Engine::Engine() {

			Util::Logger::InitializeLogger();

			windowSystem = std::make_unique<WindowSystem>(1280, 720, "Chemical");

			input = std::make_unique<Input>(windowSystem->window);
			glHandler = std::make_unique<OpenGL::Handler>();
			renderer = std::make_unique<Rendering::Renderer3D>(windowSystem->window, glHandler.get(), glm::fvec3(0.5f, 0.3f, 0.2f));

			loadedEditorScene = std::make_unique<World::Scene>(glHandler.get(), renderer.get());

			// testing code

			entt::entity e = loadedEditorScene->CreateEntity();

			std::vector<Rendering::Vertex3D> vertices = {
				Rendering::Vertex3D{glm::fvec3(-0.5f, -0.5f, 0.0f), glm::fvec3(1,0,0)},
				Rendering::Vertex3D{glm::fvec3(0.5f, -0.5f, 0.0f), glm::fvec3(0,1,0)},
				Rendering::Vertex3D{glm::fvec3(0.0f, 0.5f, 0.0f), glm::fvec3(0,0,1)}
			};

			std::vector<unsigned int> indices = {
				0,1,2
			};

			loadedEditorScene->CreateComponent<World::EditorMesh3D>(e, vertices, indices);

			loadedEditorScene->LoadScene();

			//glHandler->SubmitFunctionExecution(multiply, 5, 6);
			//handler.SubmitFunctionExecution(add, 5, 6);
			//handler.SubmitFunctionExecution(subtract, 5, 6);

			//handler.Dispatch();
		}

		Engine::~Engine() {
#ifdef CHEMICAL_DEBUG
			LOGGER_CONSOLE_MESSAGE("Engine is shutting down...");
#endif
		}

#include <GLAD/glad.h>

		void Engine::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Should be called on its own thread
			// Should only run
			glHandler->Dispatch();

			// sends rendering data to opengl thread
			renderer->Update();

			input->PollEvents();
		}
	}

}