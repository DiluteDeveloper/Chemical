#pragma once

#include <chemical/layers/Layer.h>
#include <vendor/imgui/imgui.h>
#include <chemical/core/Application.h>

class GUILayer : public Chemical::Layers::Layer {
	void OnUpdate() override {
		ImGui::Begin("Frametime");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
public:
	GUILayer() {
	}

	~GUILayer() override {
	}
};
