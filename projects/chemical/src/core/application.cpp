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

		const EventDispatcher& ApplicationData::GetDispatcher() const {
			return *m_dispatcher;
		}
		const GLFWWrapper& ApplicationData::GetGLFWWrapper() const {
			return *m_glfw_wrapper;
		}
		const GUILayer& ApplicationData::GetGUILayer() const  {
			return *m_gui_layer;
		}
		const RendererLayer& ApplicationData::GetRendererLayer() const {
			return *m_renderer_layer;
		}
		const GameLayer& ApplicationData::GetGameLayer() const {
			return *m_game_layer;
		}
		SceneLayer& ApplicationData::GetSceneLayer() const {
			return *m_scene_layer;
		}

		ApplicationData::ApplicationData() {
			m_dispatcher = new EventDispatcher();
			m_glfw_wrapper = new GLFWWrapper(m_dispatcher);

			m_scene_layer = new SceneLayer(*this);
			m_renderer_layer = new RendererLayer(*this);
			m_gui_layer = new GUILayer(*this);
			m_game_layer = new GameLayer(*this);

			m_renderer_layer->InitializeLayer();
			m_gui_layer->InitializeLayer();
			m_game_layer->InitializeLayer();
		}

		void ApplicationData::Update() {
			m_renderer_layer->UpdateLayer();
			m_gui_layer->UpdateLayer();
			m_game_layer->UpdateLayer();
		}

		ApplicationData::~ApplicationData() {
			m_game_layer->DestroyLayer();
			m_gui_layer->DestroyLayer();
			m_renderer_layer->DestroyLayer();

			delete(m_game_layer);
			delete(m_gui_layer);
			delete(m_renderer_layer);

			delete(m_glfw_wrapper);
			delete(m_dispatcher);
		}

		Application::Application() {
			m_data = new ApplicationData();

			m_data->m_dispatcher->Subscribe(WindowCloseEvent::descriptor, std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
			m_data->m_dispatcher->Subscribe(InputEvent::descriptor, std::bind(&Application::OnInput, this, std::placeholders::_1));

		}

		Application::~Application() {
			delete(m_data);
		}

		void Application::Update() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			m_data->Update();

			//game->Update(this);

			//if (running_scene != nullptr)
			//	renderer->RenderScene(*running_scene);

			//gui->Update(this);

			glfwSwapBuffers(m_data->m_glfw_wrapper->GetGLFWWindow());

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