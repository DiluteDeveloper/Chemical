#include <pch.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "gui_layer.h"
#include "scene_layer.h"
#include "scene_objects/mesh_3d.h"

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

			Scene::Object* root = m_app_data.GetLayer<SceneLayer>("SceneLayer")->GetRoot();
			if (root == nullptr)
				return;
			if (root->GetSuperiorDescriptor() != "Mesh3D")
				return;
			Scene::Mesh3D& mesh = root->GetSuperior<Scene::Mesh3D>();
			unsigned int i = 0;
			ImGui::Indent();
			ImGui::PushItemWidth(130.0f);
			ImGui::InputFloat3(std::format("position##{}", i).c_str(), &mesh.object_3d.transform.position[0]);
			ImGui::InputFloat3(std::format("rotation##{}", i).c_str(), &mesh.object_3d.transform.rotation[0]);
			ImGui::InputFloat3(std::format("scale##{}", i).c_str(), &mesh.object_3d.transform.scale[0]);
			ImGui::Unindent();

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