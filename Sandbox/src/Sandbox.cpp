#include <iostream>

#include <chemical/core/Layer.h>
#include <chemical/core/Application.h>

#include "layers/GUILayer.h"

class Sandbox : public Chemical::Core::Application {
public:
	Sandbox() {
		std::cout << "Sandbox constructor called." << std::endl;
		GetLayerStack().PushOverlay<GUILayer>(GetWindow());

	}
	~Sandbox() override {
		std::cout << "Sandbox destructor called." << std::endl;
	}
};

Chemical::Core::Application* Chemical::Core::CreateApplication() {
	return new Sandbox();
}