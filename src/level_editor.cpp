#include <spdlog/spdlog.h>
#include "level_editor.h"
#include "components.h"
#include "input.h"
#include "block/block.h"

namespace {

    using namespace Chemical;

    entt::registry* registry = nullptr;
    void OnMouseButtonCallback(int button, int action, int mods, void* data);
    void OnCursorPosCallback(double, double, void*);
    void OnScrollCallback(double, double yoffset, void*);
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

    void OnScrollCallback(double, double yoffset, void*) {
        type = (Type)(((uint8_t)type + (uint8_t)yoffset) % 5);
    }
    void OnCursorPosCallback(double, double, void*) {
        if(!dragging)
            return;

        auto view = registry->view<CharacterControllerComponent, PhysicsBodyComponent>();

        for(auto [entity, c, p] : view.each()) {
            registry->patch<BlockData>(current_drag_block, [&](BlockData& bd) {
                float distance_along_ray = (bd.start_position.z - p.transform.position.z) / c.forward.z;
                glm::fvec3 v = p.transform.position + (c.forward * distance_along_ray);
                v.x = std::round(v.x);
                v.y = std::round(v.y);
                v.z = std::round(v.z);
                bd.end_position = v;
                bd.type = type;
            });
        }

    }

    void OnMouseButtonCallback(int button, int action, int, void*) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            if(action == GLFW_PRESS) {
                auto view = registry->view<CharacterControllerComponent, PhysicsBodyComponent>();

                for(auto [entity, c, p] : view.each()) {
                    glm::fvec3 origin = p.transform.position + (c.forward * 3.0f);
                    origin.x = std::round(origin.x);
                    origin.y = std::round(origin.y);
                    origin.z = std::round(origin.z);

                    current_drag_block = registry->create();
                    registry->emplace<BlockData>(current_drag_block, BlockData{
                        .start_position = origin,
                        .end_position = origin + 1.0f,
                        .orientation = FaceOrientation::Back,
                        .type = type
                    });

                    dragging = true;
                }

            } else {
                dragging = false;
            }
        }
    }
}
