#include <pch.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "application.h"
#include "layers/renderer_layer.h"
#include "layers/gui_layer.h"
#include "layers/game_layer.h"
#include "layers/scene_layer.h"

namespace Chemical {

	namespace Core {

		
		EventDispatcher& ApplicationData::GetDispatcher() {
			return m_dispatcher;
		}
		
		GLFWWrapper& ApplicationData::GetGLFWWrapper() {
			return m_GLFWWrapper;
		}

		void ApplicationData::InitializeLayers() {
			for (auto& layer : m_layers) {
				layer->InitializeLayer(*this);
				layer->m_initialised = true;
			}
		}

		void ApplicationData::DestroyLayers() {
			for (auto& layer : m_layers) {
				layer->DestroyLayer();
			}
		}

		ApplicationData::ApplicationData() :
		m_dispatcher(), m_GLFWWrapper(m_dispatcher) {

			AddLayer<RendererLayer>("RendererLayer");
			AddLayer<SceneLayer>("SceneLayer");
			AddLayer<GUILayer>("GUILayer");
			AddLayer<GameLayer>("GameLayer");

			InitializeLayers();
		}

		void ApplicationData::Update() {
			for (auto& layer : m_layers) {
				layer->UpdateLayer();
			}
		}

		Application::Application() {

			m_data.m_dispatcher.Subscribe(WindowCloseEvent::descriptor, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
			m_data.m_dispatcher.Subscribe(InputEvent::descriptor, std::bind(&Application::OnInput, this, std::placeholders::_1));

		}

		void Application::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_data.Update();

			glfwSwapBuffers(m_data.m_GLFWWrapper.GetGLFWWindow());

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

		bool Application::IsRunning() const {
			return m_running;
		}
	}

}