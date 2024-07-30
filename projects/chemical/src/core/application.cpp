#include <pch.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <ranges>

#include "application.h"
#include "layers/renderer_layer.h"
#include "layers/gui_layer.h"
#include "layers/game_layer.h"
#include "layers/scene_layer.h"

namespace Chemical {

	namespace Core {

		EventDispatcher& ApplicationData::GetDispatcher() const {
			return *m_dispatcher;
		}
		GLFWWrapper& ApplicationData::GetGLFWWrapper() const {
			return *m_glfw_wrapper;
		}

		ApplicationData::ApplicationData() {
			m_dispatcher = new EventDispatcher();
			m_glfw_wrapper = new GLFWWrapper(*m_dispatcher);

			AddLayer<RendererLayer>("RendererLayer");
			AddLayer<SceneLayer>("SceneLayer");
			AddLayer<GUILayer>("GUILayer");
			AddLayer<GameLayer>("GameLayer");
		}

		void ApplicationData::Update() {
			for (auto& layer : m_layers) {
				layer->UpdateLayer();
			}
		}

		ApplicationData::~ApplicationData() {
			using namespace std::ranges;
			for (auto& layer : views::reverse(m_layers)) {
				delete(layer); // may have to inverse iterate
			}

			delete(m_glfw_wrapper);
			delete(m_dispatcher);
		}

		Application::Application() {

			m_data.m_dispatcher->Subscribe(WindowCloseEvent::descriptor, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
			m_data.m_dispatcher->Subscribe(InputEvent::descriptor, std::bind(&Application::OnInput, this, std::placeholders::_1));

		}

		void Application::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_data.Update();

			glfwSwapBuffers(m_data.m_glfw_wrapper->GetGLFWWindow());

			glfwPollEvents();
		}

		void Application::OnWindowClose(const Event& event) {
			m_running = false;
		}

		void Application::OnInput(const Event& event) {
			const InputEvent& event_actual = static_cast<const InputEvent&>(event);

			if (event_actual.key == GLFW_KEY_ESCAPE && event_actual.action == GLFW_PRESS) {

				if (glfwGetInputMode(event_actual.window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
					glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else
					glfwSetInputMode(event_actual.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}

		bool Application::is_running() const {
			return m_running;
		}
	}

}