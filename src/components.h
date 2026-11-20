#pragma once

#include <entt/entt.hpp>
#include "utils/transform.h"
#include "webgpu/webgpu_cpp.h"

namespace Chemical {

    enum class Direction:uint8_t {
        None =      0b0, 
        Right =     0b1, 
        Up =        0b10, 
        Forwards =  0b100, 
        Left =      0b1000, 
        Down =      0b10000, 
        Backwards = 0b100000, 
    };

    struct BoxColliderComponent {
        glm::fvec3 min;
        glm::fvec3 max;
    };
    // This is really a box collider attached to a physics object,
    // but trigger object also implies that its not ever "collided with",
    // just used for testing
    struct TriggerBoxColliderComponent {
        static constexpr float COLLISION_RESPONSE_MARGIN = 0.001f;
        glm::fvec3 min;
        glm::fvec3 max;
    };

    struct PhysicsBodyComponent {
        Transform transform = Transform();
        glm::fvec3 velocity = glm::fvec3(0);
    };

    struct CharacterControllerComponent {
        static constexpr uint32_t PHYSICS_FRAME_RATE = 60;

        float base_speed = 0.31f;
        float bhop_speed = 0.31f;
        float bhop_speed_decrease_rate = 0.5f;
        float bhop_speed_increase_rate = 0.5f;
        float jump_force = 5.5f;
        float sensitivity = 0.5f;
        bool is_grounded = false;
        double delta_accumulator = 0.0f;
        glm::fvec3 forward = glm::fvec3(1.0f);

        uint8_t last_collision_direction_flags = 0;
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

    struct CollisionResponse {
        uint8_t direction_flags = 0;
        glm::fvec3 offset = glm::fvec3(0.0);
    };

    namespace TriggerBoxColliders {
        extern bool IsColliding(const TriggerBoxColliderComponent& col, BoxColliders::ConstView other_cols);
        extern CollisionResponse GetCollisionResponse(const TriggerBoxColliderComponent& col, BoxColliders::ConstView other_cols);
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
