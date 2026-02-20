
#include "rendering.hpp"
#include <imgui/imgui.h>

namespace Chemical {
  namespace GUI {
    RenderingSettings RenderRenderingMenu(const RenderingSettings &settings) {

      ImGui::Begin("Rendering");
      ImGui::PushItemWidth(110);

      RenderingSettings s = settings;
      ImGui::InputInt("Trail Length", &s.orbit_trail_length, 100.0f, 1000.0f);
      ImGui::DragFloat("Trail Width", &s.orbit_trail_width, 0.2f, 1.0f, 15.0f,
                       "%.1f");
      ImGui::DragFloat3("Background Colour", &s.background_colour[0], 0.01f,
                        0.0f, 1.0f, "%.2f");
      ImGui::DragFloat3("Body A Colour", &s.body_a_colour[0], 0.01f, 0.0f, 1.0f,
                        "%.2f");
      ImGui::DragFloat3("Body B Colour", &s.body_b_colour[0], 0.01f, 0.0f, 1.0f,
                        "%.2f");
      ImGui::DragFloat3("Trail A Colour", &s.trail_a_colour[0], 0.01f, 0.0f,
                        1.0f, "%.2f");
      ImGui::DragFloat3("Trail B Colour", &s.trail_b_colour[0], 0.01f, 0.0f,
                        1.0f, "%.2f");
      ImGui::PopItemWidth();
      ImGui::End();

      return s;
    }
  } // namespace GUI
} // namespace Chemical
