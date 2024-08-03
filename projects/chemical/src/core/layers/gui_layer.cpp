#include <pch.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "gui_layer.h"
#include "scene_layer.h"

namespace Chemical {

	namespace Core {
		void GUILayer::InitializeLayer(ApplicationData& appData) {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(appData.GetGLFWWrapper().GetGLFWWindow(), true);

			ImGui_ImplOpenGL3_Init(appData.GetGLFWWrapper().GetGLSLVersion().c_str());
		}

		void GUILayer::UpdateLayer() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Scene Hierarchy");                          // Create a window called "Hello, world!" and append into it.

			//std::vector<Mesh3D>* meshes = m_appdata.GetLayer<SceneLayer>("SceneLayer")->GetScene()->GetMeshes3D();
			//if(meshes->size() == 0)
			//	return;
			//Mesh3D& mesh = meshes->at(0);
			//unsigned int i = 0;
			//ImGui::Indent();
			//ImGui::PushItemWidth(130.0f);
			//ImGui::InputFloat3(std::format("position##{}", i).c_str(), &mesh.transform.position[0]);
			//ImGui::InputFloat3(std::format("rotation##{}", i).c_str(), &mesh.transform.rotation[0]);
			//ImGui::InputFloat3(std::format("scale##{}", i).c_str(), &mesh.transform.scale[0]);
			//ImGui::Unindent();

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