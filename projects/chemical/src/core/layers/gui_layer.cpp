#include <pch.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "gui_layer.h"
#include "scene_layer.h"

namespace Chemical {

	namespace Core {
		void GUILayer::InitializeLayer() {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(m_app_data.GetGLFWWrapper().GetGLFWWindow(), true);

			ImGui_ImplOpenGL3_Init(m_app_data.GetGLFWWrapper().GetGLSLVersion().c_str());
		}

		void GUILayer::UpdateLayer() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Scene Hierarchy");                          // Create a window called "Hello, world!" and append into it.

			Scene* scene = m_app_data.GetLayer<SceneLayer>("SceneLayer")->GetScene();
			if (scene != nullptr) {
				unsigned int i = 0;
				for (const auto& mesh : scene->meshes) {
					ImGui::Text("Mesh %d", i + 1);
					ImGui::Indent();
					ImGui::PushItemWidth(130.0f);
					ImGui::InputFloat3(std::format("position##{}", i).c_str(), &mesh->transform.position[0]);
					ImGui::InputFloat3(std::format("rotation##{}", i).c_str(), &mesh->transform.rotation[0]);
					ImGui::InputFloat3(std::format("scale##{}", i).c_str(), &mesh->transform.scale[0]);
					ImGui::Unindent();
					i++;
				}
			}

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void GUILayer::DestroyLayer() {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}

}