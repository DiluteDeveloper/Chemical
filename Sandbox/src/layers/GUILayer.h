#pragma once

#include <chemical/core/Layer.h>
#include <vendor/imgui/imgui.h>
#include <chemical/core/Application.h>

class GUILayer : public Chemical::Core::Layer {
	void OnUpdate() override {
		ImGui::Begin("Frametime");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
public:
	GUILayer(Chemical::Core::Window& window) {
		std::cout << "GUILayer constructor called." << std::endl;
	}

	~GUILayer() override {
		std::cout << "GUILayer override destructor called." << std::endl;
	}
};
