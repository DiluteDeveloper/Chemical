#pragma once

#include "character_controller.h"
#include "level_editor.h"
#include <entt/entity/registry.hpp>
#include <imgui.h>

namespace Chemical {

    namespace GUI {
        inline void Update(entt::registry& registry) {

            auto EnabledToString = [](bool enabled) {
                if(enabled)
                    return "enabled";
                else
                    return "disabled";
            };

            auto& controller = registry.ctx().get<CharacterController>();
            ImGui::Begin("Player Configuration");

            ImGui::InputFloat("Set Speed", &controller.base_speed, 0, 5, "%.5f");
            ImGui::InputFloat("Set Bhop Speed Increase Rate", &controller.bhop_speed_increase_rate, 0, 5, "%.5f");
            ImGui::InputFloat("Set Bhop Speed Decrease Rate", &controller.bhop_speed_decrease_rate, 0, 5, "%.5f");
            ImGui::Text("Bhop speed: %.2f", controller.bhop_speed);
            ImGui::InputFloat("Set Jump Force", &controller.jump_force, 0, 5, "%.5f");
            ImGui::InputFloat("Set Gravity", &controller.gravity, 0, 5, "%.5f");
            ImGui::InputFloat("Set Sensitivity", &controller.sensitivity, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Jump Force", &jump_force, 0, 50, "%.5f");
            ImGui::InputFloat3("Set Controller Friction", &controller.controller_friction[0], 0, 1);
            ImGui::InputFloat3("Set Freecam Friction", &controller.freecam_friction[0], 0, 1);
            if(ImGui::Button(std::format("Toggle Freecam ({})", EnabledToString(controller.freecam_enabled)).c_str())) {
                controller.freecam_enabled = !controller.freecam_enabled;
            }
            auto& le = registry.ctx().get<LevelEditor>();
            if(ImGui::Button(std::format("Toggle Level Editor ({})", EnabledToString(le.IsEnabled())).c_str())) {
                le.ToggleEnabled();
            }
            ImGui::End();
        }
    }
}
