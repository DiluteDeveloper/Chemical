#include "physics.h"
#include <spdlog/spdlog.h>

namespace Chemical {

    std::vector<BoxCollider> BoxCollider::BoxColliders() {
        return {
            {{0, 0, 0}, {5, 7, 5}},
            {{0, 0, 0}, {5, 7, 5}},
            {{8, 2, 2}, {13, 9, 7}},
            // {{0, 8, 2}, {5, 15, 7}},
            {{5, 5, 5}, {20, 20, 20}},
            {{-40, 3, -40}, {40, 4, 40}},
        };
    };

    glm::fvec3 BoxCollider::TestCollisionOffsetsAgainstFirstBoxCollider(const std::vector<BoxCollider>& colliders) {
        glm::fvec3 min = colliders[0].min;
        glm::fvec3 max = colliders[0].max;

        glm::fvec3 offset = glm::fvec3(0);
        int i = 0;
        for(auto& col : colliders) {
            i++;
            if(i == 1)
                continue;

            std::array<float, 6> diffs;

            diffs[0] = col.min.x - max.x;
            if(diffs[0] > 0)
                continue;
            diffs[3] = col.max.x - min.x;
            if(diffs[3]< 0)
                continue;
            diffs[1] = col.min.y - max.y;
            if(diffs[1] > 0)
                continue;
            diffs[4] = col.max.y - min.y;
            if(diffs[4] < 0)
                continue;
            diffs[2] = col.min.z - max.z;
            if(diffs[2] > 0)
                continue;
            diffs[5] = col.max.z - min.z;
            if(diffs[5] < 0)
                continue;

            float v = 99999.0f;
            int idx = -1;
            for(size_t i = 0; i < diffs.size(); i++) {
                float abs_diff = std::abs(diffs[i]);
                if(abs_diff < v) {
                    idx = i;
                    v = abs_diff;
                }
            }
            constexpr float COLLISION_OFFSET_MARGIN = 1.0000f;
            if(idx != -1)
                offset[idx % 3] = diffs[idx] * COLLISION_OFFSET_MARGIN;
        }
        return offset;
    }
    void PhysicsBody::UpdatePhysicsBodies(std::vector<PhysicsBody>& bodies, std::vector<BoxCollider>& colliders) {

        for(auto& body : bodies) {
            body.velocity *= body.friction; 
            body.velocity.y -= body.gravity; 

            glm::fvec3 collision_response = BoxCollider::TestCollisionOffsetsAgainstFirstBoxCollider(colliders);
            body.transform.position += collision_response;
            if(collision_response.x != 0)
                body.velocity.x = 0;
            if(collision_response.y != 0)
                body.velocity.y = 0;
            if(collision_response.z != 0)
                body.velocity.z = 0;

        }
        bodies[0].transform.position += bodies[0].velocity;
        glm::fvec3 pos = bodies[0].transform.position;
        colliders[0].min = glm::fvec3(pos.x - .5, pos.y - 1.5, pos.z - .5);
        colliders[0].max = glm::fvec3(pos.x + .5, pos.y + .5, pos.z + .5);



    }

}
// #include "physics.h"
// // #include "spdlog/spdlog.h"
//
// namespace Chemical {
//
//     Transform PhysicsBody::ApplyVelocityToTransform(const Transform& _transform) {
//         Transform transform = _transform;    
//         transform.position += velocity;
//
//         return transform;
//     }
//     void PhysicsBody::ApplyNaturalForces(double delta_time) {
//         if(grounded == Grounded::Grounded)
//             velocity = velocity * glm::vec3(friction , 1, friction );
//         // else {
//         //     velocity = velocity * glm::vec3(air_resistance , 1, air_resistance);
//             // if(velocity.x > 0)
//             //     velocity.x -= velocair_resistance * delta_time;
//             // if(velocity.x < 0)
//             //     velocity.x += air_resistance * delta_time;
//             // if(velocity.z > 0)
//             //     velocity.z -= air_resistance * delta_time;
//             // if(velocity.z < 0)
//             //     velocity.z += air_resistance * delta_time;
//         // }
//     }
//     void PhysicsBody::ApplyGravity(double delta_time) {
//         velocity.y -= gravity * delta_time;
//     }
//     void PhysicsBody::ApplyVelocityForce(glm::fvec3 add_velocity) {
//         if(add_velocity.x > 0)
//             velocity.x = add_velocity.x;
//         if(add_velocity.y > 0)
//             velocity.y = add_velocity.y;
//         if(add_velocity.z > 0)
//             velocity.z = add_velocity.z;
//     }
//     void PhysicsBody::AddVelocity(glm::fvec3 add_velocity) {
//         velocity += add_velocity;
//         // glm::fvec3 add = add_velocity + velocity;
//         // if(add.x <= terminal_velocity.x && add.x >= -terminal_velocity.x)
//             // velocity.x = add.x;
//         // if(add.z <= terminal_velocity.z && add.z >= -terminal_velocity.z)
//             // velocity.z = add.z;
//         // if(add.y <= terminal_velocity.y && add.y >= -terminal_velocity.y)
//             // velocity.y = add.y;
//     }
//
//     glm::vec3 ApplyBoxCollisionResponse(PhysicsBody& body, const Extent3D& extent, const std::vector<Extent3D>& colliders) {
//
//         glm::vec3 offset = glm::vec3(0);
//         for(auto& c : colliders) {
//             // spdlog::info("ITERATION::");
//             float x_min_max_diff =  c.min.x - extent.max.x;
//             float x_max_min_diff =  c.max.x - extent.min.x;
//             // spdlog::info("x_min_max_diff: {}", x_min_max_diff);
//             // spdlog::info("x_max_min_diff: {}", x_max_min_diff);
//
//             if(x_min_max_diff > 0 || x_max_min_diff < 0)
//                 continue;
//
//             float x_offset = (std::abs(x_min_max_diff) > std::abs(x_max_min_diff)) ? 
//                 x_max_min_diff : x_min_max_diff;
//
//             float y_min_max_diff =  c.min.y - extent.max.y;
//             float y_max_min_diff =  c.max.y - extent.min.y;
//
//             // spdlog::info("y_min_max_diff: {}", y_min_max_diff);
//             // spdlog::info("y_max_min_diff: {}", y_max_min_diff);
//             if(y_min_max_diff > 0 || y_max_min_diff < 0)
//                 continue;
//
//             float y_offset = (std::abs(y_min_max_diff) > std::abs(y_max_min_diff)) ? 
//                 y_max_min_diff : y_min_max_diff;
//
//             float z_min_max_diff =  c.min.z - extent.max.z;
//             float z_max_min_diff =  c.max.z - extent.min.z;
//
//             // spdlog::info("z_min_max_diff: {}", z_min_max_diff);
//             // spdlog::info("z_max_min_diff: {}", z_max_min_diff);
//             if(z_min_max_diff > 0 || z_max_min_diff < 0)
//                 continue;
//
//             float z_offset = (std::abs(z_min_max_diff) > std::abs(z_max_min_diff)) ? 
//                 z_max_min_diff : z_min_max_diff;
//
//             if(std::abs(x_offset) < std::abs(y_offset))
//                 if(std::abs(x_offset) < std::abs(z_offset)) {
//                     body.velocity.x = 0;
//                     offset.x = x_offset * 1.01;
//                 }
//                 else {
//                     body.velocity.z = 0;
//                     offset.z = z_offset * 1.01;
//                 }
//             else
//                 if(std::abs(y_offset) < std::abs(z_offset)) {
//                     if(body.grounded != Grounded::Locked) {
//                         body.velocity.y = 0;
//                         offset.y = y_offset * 1.01;
//                     } 
//
//                 }
//                 else {
//                     body.velocity.z = 0;
//                     offset.z = z_offset * 1.01;
//                 }
//         }
//         return offset;
//     }
//     bool TestBoxCollision(const Extent3D& extent, const std::vector<Extent3D>& colliders) {
//         for(auto& c : colliders) {
//             float x_min_max_diff =  c.min.x - extent.max.x;
//             float x_max_min_diff =  c.max.x - extent.min.x;
//
//             if(x_min_max_diff > 0 || x_max_min_diff < 0)
//                 continue;
//
//             float y_min_max_diff =  c.min.y - extent.max.y;
//             float y_max_min_diff =  c.max.y - extent.min.y;
//
//             if(y_min_max_diff > 0 || y_max_min_diff < 0)
//                 continue;
//
//             float z_min_max_diff =  c.min.z - extent.max.z;
//             float z_max_min_diff =  c.max.z - extent.min.z;
//
//             if(z_min_max_diff > 0 || z_max_min_diff < 0)
//                 continue;
//
//             return true;
//         }
//         return false;
//     }
//     glm::vec3 ApplyBoxCollisionResponseBounce(const Extent3D& extent, const glm::vec3& velocity, float bounciness, const std::vector<Extent3D>& colliders) {
//         for(auto& c : colliders) {
//             // spdlog::info("ITERATION::");
//             float x_min_max_diff =  c.min.x - extent.max.x;
//             float x_max_min_diff =  c.max.x - extent.min.x;
//             // spdlog::info("x_min_max_diff: {}", x_min_max_diff);
//             // spdlog::info("x_max_min_diff: {}", x_max_min_diff);
//
//             if(x_min_max_diff > 0 || x_max_min_diff < 0)
//                 continue;
//
//             float x_offset = (std::abs(x_min_max_diff) > std::abs(x_max_min_diff)) ? 
//                 x_max_min_diff : x_min_max_diff;
//
//             float y_min_max_diff =  c.min.y - extent.max.y;
//             float y_max_min_diff =  c.max.y - extent.min.y;
//
//             // spdlog::info("y_min_max_diff: {}", y_min_max_diff);
//             // spdlog::info("y_max_min_diff: {}", y_max_min_diff);
//             if(y_min_max_diff > 0 || y_max_min_diff < 0)
//                 continue;
//
//             float y_offset = (std::abs(y_min_max_diff) > std::abs(y_max_min_diff)) ? 
//                 y_max_min_diff : y_min_max_diff;
//
//             float z_min_max_diff =  c.min.z - extent.max.z;
//             float z_max_min_diff =  c.max.z - extent.min.z;
//
//             // spdlog::info("z_min_max_diff: {}", z_min_max_diff);
//             // spdlog::info("z_max_min_diff: {}", z_max_min_diff);
//             if(z_min_max_diff > 0 || z_max_min_diff < 0)
//                 continue;
//
//             float z_offset = (std::abs(z_min_max_diff) > std::abs(z_max_min_diff)) ? 
//                 z_max_min_diff : z_min_max_diff;
//
//             if(std::abs(x_offset) < std::abs(y_offset))
//                 if(std::abs(x_offset) < std::abs(z_offset))
//                     return glm::vec3(-velocity.x * bounciness, 0, 0);
//                 else
//                     return glm::vec3(0, 0, -velocity.z * bounciness);
//
//             else
//                 if(std::abs(y_offset) < std::abs(z_offset))
//                     return glm::vec3(0, -velocity.y * bounciness, 0);
//                 else
//                     return glm::vec3(0, 0, -velocity.z * bounciness);
//         }
//         return glm::vec3(0);
//     }
// }
