#include <pch.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "master.h"

#include "core/engine.h"

namespace Chemical {

	GUI::GUI(Application* engine) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(engine->glfw_wrapper->GetGLFWWindow(), true);

		ImGui_ImplOpenGL3_Init(engine->glfw_wrapper->GetGLSLVersion().c_str());
	}

	void GUI::Update(Application* engine) {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Scene Hierarchy");                          // Create a window called "Hello, world!" and append into it.

		if (engine->running_scene != nullptr) {
			unsigned int i = 0;
			for (const auto& mesh : engine->running_scene->meshes) {
				ImGui::InputFloat3(std::format("position {}", i).c_str(), &mesh->transform.position[0]);
				ImGui::InputFloat3(std::format("rotation {}", i).c_str(), &mesh->transform.rotation[0]);
				ImGui::InputFloat3(std::format("scale {}", i).c_str(), &mesh->transform.scale[0]);

				i++;
			}
		}
		ImGui::Text("This is some useful text.");

		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}