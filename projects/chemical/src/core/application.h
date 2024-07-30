#pragma once

#include "event_dispatcher.h"
#include "glfw_wrapper.h"

namespace Chemical {

	namespace Core {

		class ApplicationData;

		class ApplicationLayer {
		public:
			friend class ApplicationData;
		protected:
			const ApplicationData& m_app_data;

			using LayerName = std::string;
		private:

			explicit ApplicationLayer(ApplicationData& app_data) :
				m_app_data(app_data) {}

			virtual void InitializeLayer() {};
			virtual void UpdateLayer() {};
			virtual void DestroyLayer() {};

		};

		class ApplicationData {
		public:
			friend class Application;

			// Returning by reference indicates that should not be nullptr.

			EventDispatcher& GetDispatcher() const;
			GLFWWrapper& GetGLFWWrapper() const;

			template<typename LayerType>
			LayerType* GetLayer(ApplicationLayer::LayerName layer_name) const {

				auto get = m_layer_ids.find(layer_name);
				if (get != m_layer_ids.end())
					return static_cast<LayerType*>(m_layers[get->second]);
				return nullptr;
			}

		private:

			ApplicationData();

			void Update();

			~ApplicationData();

			template<typename LayerType>
			void AddLayer(ApplicationLayer::LayerName layer_name) {
				ApplicationLayer* layer = static_cast<ApplicationLayer*>(new LayerType(*this));
				layer->InitializeLayer();
				m_layers.emplace_back(layer);
				m_layer_ids[layer_name] = m_layers.size() - 1;
			}

			std::vector<ApplicationLayer*> m_layers;
			std::unordered_map<ApplicationLayer::LayerName, size_t> m_layer_ids;

			EventDispatcher* m_dispatcher = nullptr;
			GLFWWrapper* m_glfw_wrapper = nullptr;
		};

		class Application {
		public:
			Application();

			void Update();

			bool is_running() const;

		private:

			ApplicationData m_data;

			bool m_running = true;

			void OnWindowClose(const Event& event);
			void OnInput(const Event& event);
		};
	}


}
