#pragma once

#include <vector>

#include "Layer.h"

namespace Chemical {
	namespace Core {


		class LayerStack {
			friend class Application;

			std::vector<Layer*> layers; // Want this to be heap allocated; cant use unique_ptr with private destructed layers,
												// has to be raw pointers manually deleted

			LayerStack(const LayerStack&) = delete;
			LayerStack(LayerStack&&) = delete;

			void Update();
			LayerStack() {};
			~LayerStack();

		public:

			template<typename T, class... ARGS>
			T& PushLayer(ARGS&&... args) { // May be unsafe
				T* t = new T(std::forward<ARGS>(args)...);
				layers.emplace_back(t);
				return *t;
			}

			
		};
	}


}

