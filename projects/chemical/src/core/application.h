#pragma once

#include "event_dispatcher.h"
#include "glfw_wrapper.h"

namespace Chemical {

	namespace Core {

		class ApplicationData;

		class ApplicationLayer {
		public:
			using LayerName = std::string;

			bool IsInitialized() {
				return m_initialised;
			}

		private:
			friend class ApplicationData;

			bool m_initialised = false;

			// store values if needed in Update or Destroy
			virtual void InitializeLayer(ApplicationData& appData) {};
			virtual void UpdateLayer() {};
			virtual void DestroyLayer() {};

		};

		class ApplicationData {
		public:

			EventDispatcher& GetDispatcher();
			GLFWWrapper& GetGLFWWrapper();

			// can return nullptr
			template<typename LayerType>
			LayerType* GetLayer(ApplicationLayer::LayerName layerName) {

				auto get = m_layerIDs.find(layerName);
				if (get != m_layerIDs.end())
					return static_cast<LayerType*>(m_layers[get->second].get());
				return nullptr;
			}

		private:
			friend class Application;

			ApplicationData();

			void Update();

			// calls InitializeLayer on all layers
			void InitializeLayers();

			// calls DestroyLayer on all layers
			void DestroyLayers();

			template<typename LayerType>
			void AddLayer(ApplicationLayer::LayerName layerName) {
				m_layers.emplace_back(std::make_unique<LayerType>());
				m_layerIDs[layerName] = m_layers.size() - 1;
			}

			// unique_ptr is used so each element can refer to different memory
			std::vector<std::unique_ptr<ApplicationLayer>> m_layers;
			std::unordered_map<ApplicationLayer::LayerName, size_t> m_layerIDs;

			EventDispatcher m_dispatcher;
			GLFWWrapper m_GLFWWrapper;
		};

		class Application {
		public:
			Application();

			void Update();

			bool IsRunning() const;

		private:

			ApplicationData m_data;

			bool m_running = true;

			void OnWindowClose(const Event& event);
			void OnInput(const Event& event);
		};
	}


}
