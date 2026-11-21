#include <spdlog/spdlog.h>
#include "level_editor.h"
#include "character_controller.h"
#include "input.h"
#include "block.h"

namespace {

    using namespace Chemical;

    entt::registry* registry = nullptr;
    void OnMouseButtonCallback(int button, int action, int mods);
    void OnCursorPosCallback(double, double);
    void OnScrollCallback(double, double yoffset);
}
namespace Chemical::LevelEditor {

    void Initialise(entt::registry& _registry) {
        registry = &_registry;

        Input::SubscribeToMouseButtonCallback(OnMouseButtonCallback, nullptr); 
        Input::SubscribeToCursorPosCallback(OnCursorPosCallback, nullptr); 
        Input::SubscribeToScrollCallback(OnScrollCallback, nullptr); 
    }

}

namespace {

    entt::entity current_drag_block;
    bool dragging = false;
    Type type = Type::Grass;

    void OnScrollCallback(double, double yoffset) {
        type = (Type)(((uint8_t)type + (uint8_t)yoffset) % 5);
    }
    void OnCursorPosCallback(double, double) {
        if(!dragging)
            return;

        auto controller = registry->ctx().get<CharacterController>();

        registry->patch<BlockData>(current_drag_block, [&](BlockData& bd) {
            float distance_along_ray = (bd.start_position.z - controller.transform.position.z) / controller.forward.z;
            glm::fvec3 v = controller.transform.position + (controller.forward * distance_along_ray);
            v.x = std::ceil(v.x);
            v.y = std::ceil(v.y);
            v.z = std::ceil(v.z);
            bd.end_position = v;
            bd.type = type;
        });

    }

    void OnMouseButtonCallback(int button, int action, int) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            if(action == GLFW_PRESS) {
                auto controller = registry->ctx().get<CharacterController>();

                glm::fvec3 origin = controller.transform.position + (controller.forward * 3.0f);
                
                // Ceil or floor depending on the orientation, and flipped for the end position
                origin.x = std::floor(origin.x);
                origin.y = std::floor(origin.y);
                origin.z = std::floor(origin.z);

                current_drag_block = registry->create();
                registry->emplace<BlockData>(current_drag_block, BlockData{
                    .start_position = origin,
                    .end_position = origin + 1.0f,
                    .orientation = FaceOrientation::Back,
                    .type = type
                });

                dragging = true;

            } else {
                dragging = false;
            }
        }
    }
}
