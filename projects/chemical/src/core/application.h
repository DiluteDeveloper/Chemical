#pragma once

//#include "rendering/renderer.h"
#include "event_dispatcher.h"
//#include "gui/master.h"
//#include "game.h"
#include "glfw_wrapper.h"

namespace Chemical {

	namespace Core {

		class ApplicationData {
		public:
			ApplicationData() :
				m_dispatcher(std::make_shared<EventDispatcher>()),
				m_glfw_wrapper(std::make_shared<GLFWWrapper>(m_dispatcher))
				//m_renderer(std::make_shared<Renderer3D>()),
				//m_gui(std::make_shared<GUI>()),
				//m_game(std::make_shared<Game>()) 
				 {}

			std::shared_ptr<EventDispatcher> GetDispatcher() {
				return m_dispatcher;
			}
			std::shared_ptr<GLFWWrapper> GetGLFWWrapper() {
				return m_glfw_wrapper;
			}
			/*std::shared_ptr<Renderer3D> GetRenderer() {
				return m_renderer;
			}
			std::shared_ptr<GUI> GetGUI() {
				return m_gui;
			}
			std::shared_ptr<Game> GetGame() {
				return m_game;
			}*/

		private:

			std::shared_ptr<EventDispatcher> m_dispatcher;
			std::shared_ptr<GLFWWrapper> m_glfw_wrapper;
			//std::shared_ptr<Renderer3D> m_renderer;
			//std::shared_ptr<GUI> m_gui;
			//std::shared_ptr<Game> m_game;
		};

		class Application {
		public:
			Application();
			~Application();

			void Update();

			bool is_running() const;

		private:

			std::shared_ptr<ApplicationData> m_data;

			//Scene* m_running_scene = nullptr;

			bool m_running = true;

			void OnWindowClose(const Event& event);
			void OnInput(const Event& event);
		};
	}


}
