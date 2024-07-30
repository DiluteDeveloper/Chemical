#pragma once

#include "event_dispatcher.h"
#include "glfw_wrapper.h"

class GameLayer;
namespace Chemical {

	namespace Core {

		class ApplicationData;

		class ApplicationLayer {
		public:
			friend class ApplicationData;
		protected:
			const ApplicationData& m_app_data;
		private:

			explicit ApplicationLayer(ApplicationData& app_data) :
				m_app_data(app_data) {}

			virtual void InitializeLayer() {};
			virtual void UpdateLayer() {};
			virtual void DestroyLayer() {};

		};

		class GUILayer;
		class RendererLayer;
		class SceneLayer;

		class ApplicationData {
		public:
			friend class Application;

			// Returning by reference indicates that should not be nullptr.

			const EventDispatcher& GetDispatcher() const;
			const GLFWWrapper& GetGLFWWrapper() const;

			const GUILayer& GetGUILayer() const;
			const RendererLayer& GetRendererLayer() const;
			const GameLayer& GetGameLayer() const;
			SceneLayer& GetSceneLayer() const;

		private:

			ApplicationData();

			void Update();

			~ApplicationData();

			GUILayer* m_gui_layer = nullptr;
			RendererLayer* m_renderer_layer = nullptr;
			SceneLayer* m_scene_layer = nullptr;
			GameLayer* m_game_layer = nullptr;

			EventDispatcher* m_dispatcher = nullptr;
			GLFWWrapper* m_glfw_wrapper = nullptr;
		};

		class Application {
		public:
			Application();
			~Application();

			void Update();

			bool is_running() const;

		private:

			ApplicationData* m_data;

			bool m_running = true;

			void OnWindowClose(const Event& event);
			void OnInput(const Event& event);
		};
	}


}
