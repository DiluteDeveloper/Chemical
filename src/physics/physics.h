#pragma once

#include <vector>

#include "utils/transform.h"

namespace Chemical {

    class PhysicsBody {
    public:
        Transform ApplyVelocityToTransform(const Transform& _transform);
        void ApplyGravity(float gravity, double delta_time = 1.0);

        void ApplyVelocityForce(glm::fvec3 add_velocity);
        void AddVelocity(glm::fvec3 add_velocity);
        glm::fvec3 velocity = glm::fvec3(0.0f);
        glm::fvec3 terminal_velocity = glm::fvec3(0.03f);
    };

    struct Extent3D {
        glm::vec3 min;
        glm::vec3 max;
    };
    extern glm::vec3 ApplyBoxCollisionResponse(const Extent3D& extent, const std::vector<Extent3D>& colliders);
    inline std::vector<Extent3D> GetTestColliderData() {
        return {
            {{0,0,0}, {5, 8, 5}},
            {{8,2,2}, {13, 9, 7}},
            {{0,8,2}, {5, 15, 7}},
            {{5,5,5}, {20, 20, 20}},
        };
    }
    extern glm::vec3 ApplyBoxCollisionResponseBounce(const Extent3D& extent, const glm::vec3& velocity, float bounciness, const std::vector<Extent3D>& colliders);
}

