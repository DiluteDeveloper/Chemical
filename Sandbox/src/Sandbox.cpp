#include <iostream>

#include <chemical/core/Application.h>
#include <chemical/core/LayerStack.h>

#include <vendor/IMGUI/imgui.h>

class Sandbox : public Chemical::Core::Application {
public:
	Sandbox() {

	}
	~Sandbox() override {
	}
};

Chemical::Core::Application* Chemical::Core::CreateApplication() {
	return new Sandbox();
}