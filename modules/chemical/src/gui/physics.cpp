#include "physics.hpp"
#include "physics/physics_body.hpp"
#include <imgui/imgui.h>
namespace Chemical {
  namespace GUI {
    void RenderPhysicsMenu(Physics::SimulationController &c) {

      ImGui::Begin("Physics");
      ImGui::PushItemWidth(200);
      if (c.IsRunning()) {
        if (ImGui::Button("Pause")) {
          c.Pause();
        }
        if (ImGui::Button("Reset")) {
          c.Reset();
        }
      } else {
        if (ImGui::Button("Play")) {
          c.Play();
        }
      }
      int speed_multiplier = c.speed_multiplier;
      ImGui::DragInt("Simulation Speed", &speed_multiplier, 20, 1, 10000, "%d",
                     ImGuiSliderFlags_Logarithmic);
      c.speed_multiplier = static_cast<unsigned int>(speed_multiplier);

      Transform &a = c.GetActiveTransformA();
      Transform &b = c.GetActiveTransformB();
      ImGui::DragFloat3("Body A Position", &a.position[0], 50, -100000, 100000,
                        "%.2f");
      ImGui::DragFloat3("Body B Position", &b.position[0], 50, -100000, 100000,
                        "%.2f");

      glm::vec3 velocity_a = c.GetActiveVelocityA();
      glm::vec3 velocity_b = c.GetActiveVelocityB();
      float mass_a = c.GetActiveMassA();
      float mass_b = c.GetActiveMassB();

      ImGui::DragFloat3("Velocity A", &velocity_a[0], .2f, -1000, 1000, "%.2f");
      ImGui::DragFloat("Mass A", &mass_a, 5000, -10000000, 10000000, "%.2f",
                       ImGuiSliderFlags_Logarithmic);
      ImGui::DragFloat3("Velocity B", &velocity_b[0], .2f, -1000, 1000, "%.2f");
      ImGui::DragFloat("Mass B", &mass_b, 5000, -10000000, 10000000, "%.2f",
                       ImGuiSliderFlags_Logarithmic);
      if (c.GetActiveMassA() != mass_a)
        c.SetActiveMassA(mass_a);
      if (c.GetActiveMassB() != mass_b)
        c.SetActiveMassB(mass_b);
      if (c.GetActiveVelocityA() != velocity_a)
        c.SetActiveVelocityA(velocity_a);
      if (c.GetActiveVelocityB() != velocity_b)
        c.SetActiveVelocityB(velocity_b);

      ImGui::PopItemWidth();
      ImGui::End();
    }
  } // namespace GUI
} // namespace Chemical
