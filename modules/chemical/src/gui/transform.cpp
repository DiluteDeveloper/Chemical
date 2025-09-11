#include "transform.hpp"
#include "imgui/imgui.h"
#include <format>

namespace Chemical {

  namespace GUI {

    void RenderTransformWindow(
        std::vector<std::pair<Transform *, std::string *>> &transforms) {

      ImGui::Begin("Transforms");
      for (auto &[transform, name] : transforms) {
        ImGui::Text("%s", name->c_str());
        ImGui::Indent();
        ImGui::Text("Position:");
        ImGui::Indent();
        ImGui::DragFloat3(std::format("##{}_position", name->c_str()).c_str(),
                          &transform->position[0], .2f, -5000, 5000, "%.2f");
        ImGui::Unindent();
        ImGui::Text("Rotation:");
        ImGui::Indent();
        ImGui::DragFloat3(std::format("##{}_rotation", name->c_str()).c_str(),
                          &transform->rotation[0], .2f, 0, 360, "%.2f");
        ImGui::Unindent();
        ImGui::Text("Scale:");
        ImGui::Indent();
        ImGui::DragFloat3(std::format("##{}_scale", name->c_str()).c_str(),
                          &transform->scale[0], .05f, 0, 1000, "%.2f");
        ImGui::Unindent();
        ImGui::Unindent();
      }
      ImGui::End();
    }
  } // namespace GUI
} // namespace Chemical
