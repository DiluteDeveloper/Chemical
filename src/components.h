#pragma once

#include <entt/entt.hpp>
#include "utils/transform.h"
#include "webgpu/webgpu_cpp.h"

namespace Chemical {

    struct BoxColliderComponent {
        glm::fvec3 min;
        glm::fvec3 max;
    };
    // This is really a box collider attached to a physics object,
    // but trigger object also implies that its not ever "collided with",
    // just used for testing
    struct TriggerBoxColliderComponent {
        glm::fvec3 min;
        glm::fvec3 max;
    };

    struct PhysicsBodyComponent {
        Transform transform = Transform();
        glm::fvec3 velocity = glm::fvec3(0);
    };

    struct CharacterControllerComponent {
        static constexpr uint32_t PHYSICS_FRAME_RATE = 60;

        float speed = 0.31f;
        float jump_force = 5.5f;
        float sensitivity = 0.5f;
        bool is_grounded = false;
        double delta_accumulator = 0.0f;

        glm::fvec3 controller_friction = glm::fvec3(0.0f);
        glm::fvec3 freecam_friction = glm::fvec3(0.0f);
        float gravity = 0.0f;
        TriggerBoxColliderComponent main_collider_offset;
        TriggerBoxColliderComponent ground_collider_offset;
        bool freecam_enabled = false;
        static void on_construct(entt::registry&, const entt::entity);
    };

    namespace BoxColliders {
        using ConstView = entt::view<entt::get_t<const BoxColliderComponent>>;
        extern void AddTestCollidersToRegistry(entt::registry& r);
    }

    namespace TriggerBoxColliders {
        extern bool IsColliding(const TriggerBoxColliderComponent& col, BoxColliders::ConstView other_cols);
        extern glm::fvec3 GetCollisionResponse(const TriggerBoxColliderComponent& col, BoxColliders::ConstView other_cols, bool& collided);
    }

    // namespace PhysicsBodies {
    //     using View = entt::view<entt::get_t<PhysicsBodyComponent, TriggerBoxColliderComponent>, entt::exclude_t<CharacterControllerComponent>>;
    //
    //     extern void Update(View bodies, BoxColliders::ConstView colliders);
    // }

    namespace CharacterControllers {
        using View = entt::view<entt::get_t<PhysicsBodyComponent, CharacterControllerComponent>>;

        extern void AddTestControllerToRegistry(entt::registry& r);
        extern void Update(View con, BoxColliders::ConstView colliders, double delta_time, wgpu::Queue queue, wgpu::Buffer buffer);
    }
}
