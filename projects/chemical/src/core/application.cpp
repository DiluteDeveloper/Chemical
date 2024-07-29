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

		void Application::WindowCloseEvent(GLFWwindow* window) {
			m_running = false;
		}

		bool Application::is_running() const {
			return m_running;
		}
	}

}