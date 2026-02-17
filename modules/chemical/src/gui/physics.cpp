#include "physics.hpp"
#include <imgui/imgui.h>
namespace Chemical {
  namespace GUI {
    void RenderPhysicsMenu(Physics::SimulationController &c) {

      ImGui::Begin("Physics");
      if (c.running) {
        if (ImGui::Button("Pause")) {
          c.running = false;
        }
        if (ImGui::Button("Stop")) {
          c.running = false;
        }
      } else {
        if (ImGui::Button("Play")) {
          c.running = true;
        }
      }

      ImGui::End();
    }
  } // namespace GUI
} // namespace Chemical
