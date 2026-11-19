#pragma once

#include <entt/entt.hpp>
#include "utils/transform.h"

namespace Chemical {

    struct BoxColliderComponent {
        glm::fvec3 min;
        glm::fvec3 max;
    };

    struct PhysicsBodyComponent {
        Transform transform = Transform();
        glm::fvec3 velocity = glm::fvec3(0);
        const float friction = 1.0f;

        const glm::fvec3 collider_min_extent = glm::vec3(1.0f);
        const glm::fvec3 collider_max_extent = glm::vec3(1.0f);
        const float gravity = 0.0f;
    };

    namespace BoxColliders {
        using ConstView = entt::view<entt::get_t<const BoxColliderComponent>>;
        extern void AddTestCollidersToRegistry(entt::registry& r);
    }

    namespace PhysicsBodies {
        using View = entt::view<entt::get_t<PhysicsBodyComponent>>;

        extern void Update(View bodies, BoxColliders::ConstView colliders);
    }


    // Only does physicsbody 0 for now, assuming extent3d 0 is also for physicsbody 0

}
// #pragma once
//
// #include <vector>
//
// #include "utils/transform.h"
//
// namespace Chemical {
//
//     enum class Grounded {
//         Grounded = 1,
//         NotGrounded = 0,
//         Locked = 2
//     };
//     class PhysicsBody {
//     public:
//         Transform ApplyVelocityToTransform(const Transform& _transform);
//         void ApplyNaturalForces(double delta_time = 1.0);
//         void ApplyGravity(double delta_time = 1.0);
//
//         void ApplyVelocityForce(glm::fvec3 add_velocity);
//         void AddVelocity(glm::fvec3 add_velocity);
//         glm::fvec3 velocity = glm::fvec3(0.0f);
//         glm::fvec3 terminal_velocity = glm::fvec3(0.03f);
//         float gravity = 0.11f;
//         float friction = 0.96;
//         float air_speed_increase = 1.01;
//
//         Grounded grounded = Grounded::NotGrounded;
//     };
//
//     struct Extent3D {
//         glm::vec3 min;
//         glm::vec3 max;
//     };
//     extern glm::vec3 ApplyBoxCollisionResponse(glm::vec3& velocity, const Extent3D& extent, const std::vector<Extent3D>& colliders);
//     extern bool TestBoxCollision(const Extent3D& extent, const std::vector<Extent3D>& colliders);
//     inline std::vector<Extent3D> GetTestColliderData() {
//         return {
//             {{0,0,0}, {5, 7, 5}},
//             {{8,2,2}, {13, 9, 7}},
//             // {{0,8,2}, {5, 15, 7}},
//             {{5,5,5}, {20, 20, 20}},
//             {{-40,3,-40}, {40, 4, 40}},
//         };
//     }
//     extern glm::vec3 ApplyBoxCollisionResponseBounce(const Extent3D& extent, const glm::vec3& velocity, float bounciness, const std::vector<Extent3D>& colliders);
// }
//
