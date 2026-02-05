#include "terrain_state_menu.hpp"

#include "imgui/imgui.h"

namespace Chemical {
  namespace GUI {
    void RenderTerrainStateMenu(Util::TerrainState &terrain) {

      ImGui::Begin("Terrain State");
      if (ImGui::Button("Regenerate Terrain")) {
        terrain.Generate();
      }
      glm::ivec3 size = terrain.size;
      ImGui::SliderInt3("Terrain Size", &size[0], 1, 1000);
      terrain.size = size;
      ImGui::SliderFloat("Terrain Amplitude", &terrain.noise_amplitude, 0.05f,
                         10);
      ImGui::SliderFloat("Terrain Frequency", &terrain.noise_frequency, 0.01f,
                         1);
      ImGui::SliderFloat("Terrain Isovalue", &terrain.iso_value, 0.1f, 0.9f);
      ImGui::End();
    }
  } // namespace GUI
} // namespace Chemical
