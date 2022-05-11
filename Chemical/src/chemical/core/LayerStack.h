#pragma once

#include <vector>
#include <iostream>

#include "AccessKey.h"

namespace Chemical {

	namespace Core {
		class Layer;
		class Application;

		class LayerStack {

			std::vector<Layer*> layers;	

			LayerStack(const LayerStack&) = delete;
			LayerStack(LayerStack&&) = delete;

		public:
			void Update(AccessKey<Application>);
			LayerStack(AccessKey<Application>) { std::cout << "LayerStack constructor called." << std::endl; };
			~LayerStack();

			template<typename T, class... ARGS>
			void PushLayer(ARGS&&... args) {
				layers.insert(layers.begin(), new T(std::forward<ARGS>(args)...));
			}
			template<typename T, class... ARGS>
			void PushOverlay(ARGS&&... args) {
				layers.emplace_back(new T(std::forward<ARGS>(args)...));
			}

			
		};
	}

}

