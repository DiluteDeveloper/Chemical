#include "terrain_state_menu.hpp"

#include "imgui/imgui.h"
#include "terrain_state.hpp"

namespace Chemical {
  namespace GUI {
    void RenderTerrainStateMenu(Util::TerrainState &terrain) {

      ImGui::Begin("Terrain State");
      if (ImGui::Button("Regenerate Terrain")) {
        terrain.Generate();
      }
      if (ImGui::Button("Add New Noise Octave")) {
        terrain.AddNoiseOctave(Util::TerrainState::NoiseOctave{});
      }
      if (ImGui::Button("Pop Last Noise Octave")) {
        terrain.RemoveLastNoiseOctave();
      }
      glm::ivec3 size = terrain.size;
      ImGui::SliderInt3("Terrain Size", &size[0], 1, 1000);
      terrain.size = size;
      ImGui::SliderFloat("Terrain Isovalue", &terrain.iso_value, 0.1f, 0.9f);

      for (unsigned int i = 0; i < terrain.GetNoiseOctaveCount(); i++) {
        ImGui::Text("%s", std::format("Octave {} : ", i).c_str());
        ImGui::Indent();
        ImGui::SliderFloat(std::format("Frequency###Octfreq{}", i).c_str(),
                           &terrain.GetNoiseOctave(i).first, 0.01f, .5f);
        ImGui::SliderFloat(std::format("Amplitude###Octamp{}", i).c_str(),
                           &terrain.GetNoiseOctave(i).second, 0.05f, 5);
        ImGui::Unindent();
      }

      ImGui::End();
    }
  } // namespace GUI
} // namespace Chemical
