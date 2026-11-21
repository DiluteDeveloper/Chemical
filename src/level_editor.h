#pragma once

#include "block.h"
#include <entt/entity/registry.hpp>

namespace Chemical {

    class LevelEditor {
    public:
        LevelEditor(entt::registry& registry);
        ~LevelEditor();
    private:
        entt::registry* registry;
        void OnMouseButtonCallback(int button, int action, int mods);
        void OnCursorPosCallback(double, double);
        void OnScrollCallback(double, double yoffset);

        entt::entity current_drag_block;
        bool dragging = false;
        BlockType type = BlockType::Grass;
    };
}
