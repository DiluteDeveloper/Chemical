#include "LayerStack.h"

namespace Chemical {

	namespace Core {

		void LayerStack::Update() {
			for (Layer* l : layers)
				l->OnUpdate();
		}

		LayerStack::~LayerStack() {

			for (Layer* l : layers) {
				delete l;
			}
			layers.clear();
		}
	}

}

