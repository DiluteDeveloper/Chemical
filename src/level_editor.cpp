#include <spdlog/spdlog.h>
#include "level_editor.h"
#include "components.h"

namespace Chemical {

    void LevelEditor::Update(const entt::registry& r) {
        auto view = r.view<CharacterControllerComponent, PhysicsBodyComponent>();

        for(auto [entity, c, p] : view.each()) {
            glm::fvec3 mult = c.forward * 3.0f;
            glm::fvec3 result = p.transform.position + mult;
            spdlog::info("{}, {}, {}", result.x, result.y, result.z);
        }
    }
}

