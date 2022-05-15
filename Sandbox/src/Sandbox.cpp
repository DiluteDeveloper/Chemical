#include <iostream>

#include <chemical/layers/Layer.h>
#include <chemical/core/Application.h>
#include <chemical/core/LayerStack.h>

#include "layers/GUILayer.h"

class Sandbox : public Chemical::Core::Application {
public:
	Sandbox() {
		layerStack->PushOverlay<GUILayer>();

	}
	~Sandbox() override {
	}
};

Chemical::Core::Application* Chemical::Core::CreateApplication() {
	return new Sandbox();
}