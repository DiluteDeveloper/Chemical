#pragma once

#include "block.h"
#include <entt/entity/registry.hpp>

namespace Chemical {

    class LevelEditor {
    public:
        LevelEditor(entt::registry& registry);
        ~LevelEditor();

        void ToggleEnabled();
        bool IsEnabled();

    private:
        bool enabled = false;
        entt::registry* registry;
        void OnMouseButtonCallback(int button, int action, int mods);
        void OnCursorPosCallback(double, double);
        void OnScrollCallback(double, double yoffset);
        void OnKeyCallback(int, int, int, int);

        struct DragBlock {
            entt::entity entity;
            std::array<int32_t*, 2> drag_pos_ptr;
            std::array<uint8_t, 2> varying_dimensions;
            uint8_t locked_dimension;
            std::array<bool, 2> start_and_end_swapped;
            std::array<std::function<float(float)>, 2> ceil_or_floor;
            BlockType cached_type = BlockType::Dirt;
            FaceOrientation cached_orientation = FaceOrientation::Back;
        } drag_block;

        bool dragging = false;

        [[nodiscard]] static float CeilNotStupidSTDTemplates(float);
        [[nodiscard]] static float FloorNotStupidSTDTemplates(float);

        void CreateNewDragBlock(const BlockData& previous);
    };
}
