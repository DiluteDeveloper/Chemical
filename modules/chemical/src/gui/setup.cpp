#include "setup.hpp"

#include "glfw/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace Chemical {

  namespace GUI {
    void SetupGUI(GLFWwindow *window) {
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGuiIO &io = ImGui::GetIO();
      (void)io;
      io.ConfigFlags |=
          ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
      io.ConfigFlags |=
          ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

      // Setup Dear ImGui style
      ImGui::StyleColorsDark();
      // ImGui::StyleColorsLight();

      // Setup scaling
      ImGuiStyle &style = ImGui::GetStyle();

      // Setup Platform/Renderer backends
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 450");
    }
    void CleanupGUI() {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
    }
    void BeginFrame() {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
    }
    void RenderFrame() {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

  } // namespace GUI

} // namespace Chemical
