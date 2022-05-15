#include "LayerStack.h"
#include "chemical/layers/Layer.h"

namespace Chemical {

	namespace Core {

		void LayerStack::Update() {
			for (Layers::Layer* l : layers)
				l->OnUpdate();
		}

		LayerStack::~LayerStack() {

			for (Layers::Layer* l : layers) {
				delete l;
			}
			layers.clear();
		}
	}

}

