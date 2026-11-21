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
        input.SubscribeToKeyCallback(std::bind_front(&LevelEditor::OnKeyCallback, this), this); 
    }
    LevelEditor::~LevelEditor() {

        Input& input = registry->ctx().get<Input>();
        input.UnsubscribeFromMouseButtonCallback(this); 
        input.UnsubscribeFromCursorPosCallback(this); 
        input.UnsubscribeFromScrollCallback(this); 
        input.UnsubscribeFromKeyCallback(this); 
    }

    void LevelEditor::OnKeyCallback(int key, int, int action, int) {
        CharacterController& c = registry->ctx().get<CharacterController>();
        if(!c.input_enabled) return;
        if(!enabled) return;
        if(key == GLFW_KEY_R && action == GLFW_PRESS) {
            registry->patch<BlockData>(drag_block.entity, [&](BlockData& bd) {
                bd.orientation = (FaceOrientation)(((uint8_t)bd.orientation + 1) % 6);
            });
        }
    }
    void LevelEditor::OnScrollCallback(double, double yoffset) {
        CharacterController& c = registry->ctx().get<CharacterController>();
        if(!c.input_enabled) return;
        if(!enabled) return;
        registry->patch<BlockData>(drag_block.entity, [&](BlockData& bd) {
            bd.type = (BlockType)(((int8_t)bd.type + (int8_t)yoffset) % 7);
        });
    }
    void LevelEditor::OnCursorPosCallback(double, double) {
        CharacterController& c = registry->ctx().get<CharacterController>();
        if(!c.input_enabled) return;
        if(!enabled) return;
        if(!dragging) {

            auto controller = registry->ctx().get<CharacterController>();

            glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
            
            BlockData& bd = registry->get<BlockData>(drag_block.entity);

            origin.x = std::floor(origin.x);
            origin.y = std::floor(origin.y);
            origin.z = std::floor(origin.z);

            // For performance while im just regenerating the entire buffer each time a block changes
            if(glm::ivec3(origin) == bd.start_position)
                return;

            glm::fvec3 end_pos = origin;
            std::array<uint8_t, 2> varying_dims = FaceOrientationToVaryingDimensionsIdx(bd.orientation);
            end_pos[varying_dims[0]] += 1;
            end_pos[varying_dims[1]] += 1;

            registry->patch<BlockData>(drag_block.entity, [&](BlockData&) {
                bd.start_position = origin;
                bd.end_position = end_pos;
            });

            return;
        }

        auto controller = registry->ctx().get<CharacterController>();

        auto swap = [](int32_t& a, int32_t& b) {
            int32_t v = a;
            a = b;
            b = v;
        };
        BlockData& bd = registry->get<BlockData>(drag_block.entity);
        float distance_along_ray = (bd.start_position[drag_block.locked_dimension] - controller.transform.position[drag_block.locked_dimension]) / controller.forward[drag_block.locked_dimension];
        glm::fvec3 v = controller.transform.position + (controller.forward * distance_along_ray);
        int v_fixed_0 = drag_block.ceil_or_floor[0](v[drag_block.varying_dimensions[0]]);
        int v_fixed_1 = drag_block.ceil_or_floor[1](v[drag_block.varying_dimensions[1]]);

        if(*drag_block.drag_pos_ptr[0] == v_fixed_0 && *drag_block.drag_pos_ptr[1] == v_fixed_1)
            return;
        registry->patch<BlockData>(drag_block.entity, [&](BlockData&) {
            *drag_block.drag_pos_ptr[0] = v_fixed_0;
            *drag_block.drag_pos_ptr[1] = v_fixed_1;

            if(bd.end_position[drag_block.varying_dimensions[0]] <= bd.start_position[drag_block.varying_dimensions[0]]) {
                swap(bd.end_position[drag_block.varying_dimensions[0]], bd.start_position[drag_block.varying_dimensions[0]]);
                if(drag_block.start_and_end_swapped[0]) {
                    drag_block.drag_pos_ptr[0] = &bd.end_position[drag_block.varying_dimensions[0]];
                    drag_block.ceil_or_floor[0] = CeilNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[0] = drag_block.ceil_or_floor[0](v[drag_block.varying_dimensions[0]]);
                    bd.start_position[drag_block.varying_dimensions[0]] -= 1;
                } else {
                    drag_block.drag_pos_ptr[0] = &bd.start_position[drag_block.varying_dimensions[0]];
                    drag_block.ceil_or_floor[0] = FloorNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[0] = drag_block.ceil_or_floor[0](v[drag_block.varying_dimensions[0]]);
                    bd.end_position[drag_block.varying_dimensions[0]] += 1;
                }
                drag_block.start_and_end_swapped[0] = !drag_block.start_and_end_swapped[0];

            }
            if(bd.end_position[drag_block.varying_dimensions[1]] <= bd.start_position[drag_block.varying_dimensions[1]]) {
                swap(bd.end_position[drag_block.varying_dimensions[1]], bd.start_position[drag_block.varying_dimensions[1]]);
                if(drag_block.start_and_end_swapped[1]) {
                    drag_block.drag_pos_ptr[1] = &bd.end_position[drag_block.varying_dimensions[1]];
                    drag_block.ceil_or_floor[1] = CeilNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[1] = drag_block.ceil_or_floor[1](v[drag_block.varying_dimensions[1]]);
                    bd.start_position[drag_block.varying_dimensions[1]] -= 1;
                } else {
                    drag_block.drag_pos_ptr[1] = &bd.start_position[drag_block.varying_dimensions[1]];
                    drag_block.ceil_or_floor[1] = FloorNotStupidSTDTemplates;
                    *drag_block.drag_pos_ptr[1] = drag_block.ceil_or_floor[1](v[drag_block.varying_dimensions[1]]);
                    bd.end_position[drag_block.varying_dimensions[1]] += 1;
                }
                drag_block.start_and_end_swapped[1] = !drag_block.start_and_end_swapped[1];
            }

        });

    }

    bool LevelEditor::IsEnabled() {
        return enabled;
    }
    void LevelEditor::ToggleEnabled() {

        if(enabled) {
            BlockData& bd = registry->get<BlockData>(drag_block.entity);
            drag_block.cached_type = bd.type;
            drag_block.cached_orientation = bd.orientation;
            registry->destroy(drag_block.entity);
        } else {
            auto controller = registry->ctx().get<CharacterController>();

            glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
            glm::fvec3 end_pos = origin;
            std::array<uint8_t, 2> varying_dims = FaceOrientationToVaryingDimensionsIdx(drag_block.cached_orientation);
            end_pos[varying_dims[0]] += 1;
            end_pos[varying_dims[1]] += 1;
            
            origin.x = std::floor(origin.x);
            origin.y = std::floor(origin.y);
            origin.z = std::floor(origin.z);

            BlockData bd = {
                .start_position = origin,
                .end_position = origin + 1.0f,
                .orientation = drag_block.cached_orientation,
                .type = drag_block.cached_type
            };
            CreateNewDragBlock(bd);
        }
        dragging = false;
        enabled = !enabled;
    }
    void LevelEditor::CreateNewDragBlock(const BlockData& previous) {
        drag_block.entity = registry->create();
        registry->emplace<BlockData>(drag_block.entity, previous);
    }
    void LevelEditor::OnMouseButtonCallback(int button, int action, int) {
        CharacterController& c = registry->ctx().get<CharacterController>();
        if(!c.input_enabled) return;
        if(!enabled) return;
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            if(action == GLFW_PRESS) {
                auto controller = registry->ctx().get<CharacterController>();

                glm::fvec3 origin = controller.transform.position + (controller.forward * 5.0f);
                
                origin.x = std::floor(origin.x);
                origin.y = std::floor(origin.y);
                origin.z = std::floor(origin.z);


                BlockData& bd = registry->get<BlockData>(drag_block.entity);
                drag_block.varying_dimensions = FaceOrientationToVaryingDimensionsIdx(bd.orientation);
                drag_block.locked_dimension = FaceOrientationToLockedDimensionIdx(bd.orientation);
                drag_block.drag_pos_ptr[0] = &bd.end_position[drag_block.varying_dimensions[0]];
                drag_block.drag_pos_ptr[1] = &bd.end_position[drag_block.varying_dimensions[1]];
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
