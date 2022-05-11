#include "LayerStack.h"
#include "Layer.h"

namespace Chemical {

	namespace Core {

		void LayerStack::Update(AccessKey<Application>) {
			for (Layer* l : layers)
				l->OnUpdate();
		}

		LayerStack::~LayerStack() {
			std::cout << "LayerStack destructor called." << std::endl;

			for (Layer* l : layers) {
				delete l;
			}
			layers.clear();
		}
	}

}

