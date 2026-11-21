#pragma once

#include <bitset>
#include <glm/glm.hpp>
#include <entt/entt.hpp>

namespace Chemical {

    // Used as a bitset
    enum class Direction:uint8_t {
        None =      0, 
        Right =     1, 
        Up =        2, 
        Forwards =  3, 
        Left =      4, 
        Down =      5, 
        Backwards = 6, 
    };

    struct BoxCollider {
        glm::fvec3 min;
        glm::fvec3 max;
        static void AddTestCollidersToRegistry(entt::registry& r);
    };

    struct CollisionResponse {
        std::bitset<8> direction_flags = 0;
        glm::fvec3 offset = glm::fvec3(0.0);
    };

    // This is really a box collider attached to a physics object,
    // but trigger object also implies that its not ever "collided with",
    // just used for testing
    struct TriggerBoxCollider {
        static constexpr float COLLISION_RESPONSE_MARGIN = 0.001f;
        glm::fvec3 min;
        glm::fvec3 max;

        using BoxColliderConstView = entt::view<entt::get_t<const BoxCollider>>;
        static bool IsColliding(const TriggerBoxCollider& trigger, BoxColliderConstView colliders);
        static CollisionResponse GetCollisionResponse(const TriggerBoxCollider& trigger, BoxColliderConstView colliders);
    };

}
