#include <pch.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "gui.h"

#include "core/window.h"

namespace Chemical {
	namespace GUI {
		void InitializeGUI() {
			IMGUI_CHECKVERSION();
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO(); (void)io;
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

			// ImGui::StyleColorsDark();

			ImGui_ImplGlfw_InitForOpenGL(Core::Window::glfw_window, true);
			ImGui_ImplOpenGL3_Init(Core::Window::glsl_version.c_str());
		}

		void NewFrame() {
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
		}
		void Render() {
			ImGui::Render();

			// Might need to split these two up?

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
}