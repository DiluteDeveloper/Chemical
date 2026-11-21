#include <spdlog/spdlog.h>
#include "level_editor.h"
#include "character_controller.h"
#include "input.h"
#include "block.h"

namespace Chemical {

    LevelEditor::LevelEditor(entt::registry& registry) : registry(&registry) {

        Input& input = registry.ctx().get<Input>();
        input.SubscribeToMouseButtonCallback(std::bind_front(&LevelEditor::OnMouseButtonCallback, this), this); 
        input.SubscribeToCursorPosCallback(std::bind_front(&LevelEditor::OnCursorPosCallback, this), this); 
        input.SubscribeToScrollCallback(std::bind_front(&LevelEditor::OnScrollCallback, this), this); 

        auto controller = registry.ctx().get<CharacterController>();

        glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
        
        origin.x = std::floor(origin.x);
        origin.y = std::floor(origin.y);
        origin.z = std::floor(origin.z);

        BlockData bd = {
            .start_position = origin,
            .end_position = origin + 1.0f,
            .orientation = FaceOrientation::Back,
            .type = BlockType::Dirt
        };
        CreateNewDragBlock(bd);
    }
    LevelEditor::~LevelEditor() {

        Input& input = registry->ctx().get<Input>();
        input.UnsubscribeFromMouseButtonCallback(this); 
        input.UnsubscribeFromCursorPosCallback(this); 
        input.UnsubscribeFromScrollCallback(this); 
    }

    void LevelEditor::OnScrollCallback(double, double yoffset) {
        registry->patch<BlockData>(drag_block.entity, [&](BlockData& bd) {
            bd.type = (BlockType)(((uint8_t)bd.type + (uint8_t)yoffset) % 5);
        });
    }
    void LevelEditor::OnCursorPosCallback(double, double) {
        if(!dragging) {

            auto controller = registry->ctx().get<CharacterController>();

            glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
            
            origin.x = std::floor(origin.x);
            origin.y = std::floor(origin.y);
            origin.z = std::floor(origin.z);
            registry->patch<BlockData>(drag_block.entity, [&](BlockData& bd) {
                bd.start_position = origin;
                bd.end_position = origin + 1.0f;
            });

            return;
        }

        auto controller = registry->ctx().get<CharacterController>();

        auto swap = [](int32_t& a, int32_t& b) {
            int32_t v = a;
            a = b;
            b = v;
        };
        registry->patch<BlockData>(drag_block.entity, [&](BlockData& bd) {
            float distance_along_ray = (bd.start_position.z - controller.transform.position.z) / controller.forward.z;
            glm::fvec3 v = controller.transform.position + (controller.forward * distance_along_ray);
            *drag_block.drag_pos_ptr[0] = drag_block.ceil_or_floor[0](v[drag_block.dimension_idx[0]]);
            *drag_block.drag_pos_ptr[1] = drag_block.ceil_or_floor[1](v[drag_block.dimension_idx[1]]);

            if(bd.end_position[drag_block.dimension_idx[0]] <= bd.start_position[drag_block.dimension_idx[0]]) {
                swap(bd.end_position[drag_block.dimension_idx[0]], bd.start_position[drag_block.dimension_idx[0]]);
                if(drag_block.start_and_end_swapped[0]) {
                    drag_block.drag_pos_ptr[0] = &bd.end_position[drag_block.dimension_idx[0]];
                    drag_block.ceil_or_floor[0] = CeilNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[0] = drag_block.ceil_or_floor[0](v[drag_block.dimension_idx[0]]);
                    bd.start_position[drag_block.dimension_idx[0]] -= 1;
                } else {
                    drag_block.drag_pos_ptr[0] = &bd.start_position[drag_block.dimension_idx[0]];
                    drag_block.ceil_or_floor[0] = FloorNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[0] = drag_block.ceil_or_floor[0](v[drag_block.dimension_idx[0]]);
                    bd.end_position[drag_block.dimension_idx[0]] += 1;
                }
                drag_block.start_and_end_swapped[0] = !drag_block.start_and_end_swapped[0];

            }
            if(bd.end_position[drag_block.dimension_idx[1]] <= bd.start_position[drag_block.dimension_idx[1]]) {
                swap(bd.end_position[drag_block.dimension_idx[1]], bd.start_position[drag_block.dimension_idx[1]]);
                if(drag_block.start_and_end_swapped[1]) {
                    drag_block.drag_pos_ptr[1] = &bd.end_position[drag_block.dimension_idx[1]];
                    drag_block.ceil_or_floor[1] = CeilNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[1] = drag_block.ceil_or_floor[1](v[drag_block.dimension_idx[1]]);
                    bd.start_position[drag_block.dimension_idx[1]] -= 1;
                } else {
                    drag_block.drag_pos_ptr[1] = &bd.start_position[drag_block.dimension_idx[1]];
                    drag_block.ceil_or_floor[1] = FloorNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[1] = drag_block.ceil_or_floor[1](v[drag_block.dimension_idx[1]]);
                    bd.end_position[drag_block.dimension_idx[1]] += 1;
                }
                drag_block.start_and_end_swapped[1] = !drag_block.start_and_end_swapped[1];
            }

        });

    }

    void LevelEditor::CreateNewDragBlock(const BlockData& previous) {
        drag_block.entity = registry->create();
        registry->emplace<BlockData>(drag_block.entity, previous);
    }
    void LevelEditor::OnMouseButtonCallback(int button, int action, int) {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            if(action == GLFW_PRESS) {
                auto controller = registry->ctx().get<CharacterController>();

                glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
                
                origin.x = std::floor(origin.x);
                origin.y = std::floor(origin.y);
                origin.z = std::floor(origin.z);


                BlockData& bd = registry->get<BlockData>(drag_block.entity);
                drag_block.dimension_idx = FaceOrientationToVaryingDimensionsIdx(FaceOrientation::Back);
                drag_block.drag_pos_ptr[0] = &bd.end_position[drag_block.dimension_idx[0]];
                drag_block.drag_pos_ptr[1] = &bd.end_position[drag_block.dimension_idx[1]];
                drag_block.start_and_end_swapped = {false, false};
                drag_block.ceil_or_floor = {LevelEditor::CeilNotStupidSTDTemplates, LevelEditor::CeilNotStupidSTDTemplates};

                dragging = true;

            } else {
                dragging = false;

                BlockData& bd = registry->get<BlockData>(drag_block.entity);
                CreateNewDragBlock(bd);
            }
        }
    }
        [[nodiscard]] float LevelEditor::CeilNotStupidSTDTemplates(float x) {
            return std::ceil(x);
        }
        [[nodiscard]] float LevelEditor::FloorNotStupidSTDTemplates(float x) {
            return std::floor(x);
        }
}
