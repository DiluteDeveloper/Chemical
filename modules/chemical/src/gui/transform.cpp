#include "transform.hpp"
#include "imgui/imgui.h"
#include <format>

namespace Chemical {

  namespace GUI {

    int RenderTransformWindow(
        std::vector<std::pair<Transform *, std::string *>> &transforms) {

      int ret = -1;

      ImGui::Begin("Transforms");
      int i = 0;
      for (auto &[transform, name] : transforms) {
        ImGui::Text("%s", name->c_str());
        ImGui::Indent();
        if (ImGui::Button(std::format("Select##{}", name->c_str()).c_str())) {
          ret = i;
        }
        ImGui::Text("Position:");
        ImGui::Indent();
        ImGui::DragFloat3(std::format("##{}_position", name->c_str()).c_str(),
                          &transform->position[0], .05f, -5000, 5000, "%.2f");
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

        i++;
      }
      ImGui::End();

      return ret;
    }
  } // namespace GUI
} // namespace Chemical
