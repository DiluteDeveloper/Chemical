#include <pch.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "console_logger.h"

namespace Chemical {

	namespace Core {
		Application::Application() {

			ConsoleLogger::Initialize();

			m_data = std::make_shared<ApplicationData>();

			m_data->GetDispatcher()->Subscribe(WindowCloseEvent::descriptor, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
			m_data->GetDispatcher()->Subscribe(InputEvent::descriptor, std::bind(&Application::OnInput, this, std::placeholders::_1));

		}
		Application::~Application() {
			glfwTerminate();
		}

		void Application::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//game->Update(this);

			//if (running_scene != nullptr)
			//	renderer->RenderScene(*running_scene);

			//gui->Update(this);

			glfwSwapBuffers(m_data->GetGLFWWrapper()->GetGLFWWindow());

			glfwPollEvents();
		}

		void Application::OnWindowClose(const Event& event) {
			m_running = false;
		}

		void Application::OnInput(const Event& event) {
			const InputEvent& event_actual = static_cast<const InputEvent&>(event);

			if (event_actual.key == GLFW_KEY_ESCAPE && event_actual.action == GLFW_PRESS) {

				//Camera& camera = renderer->GetCamera();
				if (glfwGetInputMode(event_actual.window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
					glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else
					glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				//camera.SetEnabled(!camera.GetEnabled());
			}
		}

		bool Application::is_running() const {
			return m_running;
		}
	}

}