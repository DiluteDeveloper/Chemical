#pragma once

#include <vector>

namespace Chemical {

	namespace Layers {
		class Layer;
	}
	namespace Core {


		class LayerStack {
			friend class Application;

			std::vector<Layers::Layer*> layers;	

			LayerStack(const LayerStack&) = delete;
			LayerStack(LayerStack&&) = delete;

			void Update();
			LayerStack() = default;
			~LayerStack();

		public:

			template<typename T, class... ARGS>
			T& PushLayer(ARGS&&... args) { // May be unsafe
				layers.insert(layers.begin(), new T(std::forward<ARGS>(args)...));
				return static_cast<T&>(*layers[0]);
			}
			template<typename T, class... ARGS>
			T& PushOverlay(ARGS&&... args) { // May be unsafe
				layers.emplace_back(new T(std::forward<ARGS>(args)...));
				return static_cast<T&>(*layers[layers.size() - 1]);
			}

			
		};
	}

}

