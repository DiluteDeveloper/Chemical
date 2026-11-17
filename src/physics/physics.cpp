#include "physics.h"
// #include "spdlog/spdlog.h"

namespace Chemical {

    Transform PhysicsBody::ApplyVelocityToTransform(const Transform& _transform) {
        Transform transform = _transform;    
        transform.position += velocity;
        velocity = velocity * glm::vec3(0.97, 1, 0.97);
        return transform;
    }
    void PhysicsBody::ApplyGravity(float gravity, double delta_time) {
        velocity.y -= gravity * delta_time;
    }
    void PhysicsBody::ApplyVelocityForce(glm::fvec3 add_velocity) {
        if(add_velocity.x > 0)
            velocity.x = add_velocity.x;
        if(add_velocity.y > 0)
            velocity.y = add_velocity.y;
        if(add_velocity.z > 0)
            velocity.z = add_velocity.z;
    }
    void PhysicsBody::AddVelocity(glm::fvec3 add_velocity) {
        glm::fvec3 add = add_velocity + velocity;
        if(add.x <= terminal_velocity.x)
            velocity.x = add.x;
        if(add.y <= terminal_velocity.y)
            velocity.y = add.y;
        if(add.z <= terminal_velocity.z)
            velocity.z = add.z;
    }

    glm::vec3 ApplyBoxCollisionResponse(PhysicsBody& body, const Extent3D& extent, const std::vector<Extent3D>& colliders) {

        glm::vec3 offset = glm::vec3(0);
        for(auto& c : colliders) {
            // spdlog::info("ITERATION::");
            float x_min_max_diff =  c.min.x - extent.max.x;
            float x_max_min_diff =  c.max.x - extent.min.x;
            // spdlog::info("x_min_max_diff: {}", x_min_max_diff);
            // spdlog::info("x_max_min_diff: {}", x_max_min_diff);

            if(x_min_max_diff > 0 || x_max_min_diff < 0)
                continue;

            float x_offset = (std::abs(x_min_max_diff) > std::abs(x_max_min_diff)) ? 
                x_max_min_diff : x_min_max_diff;

            float y_min_max_diff =  c.min.y - extent.max.y;
            float y_max_min_diff =  c.max.y - extent.min.y;

            // spdlog::info("y_min_max_diff: {}", y_min_max_diff);
            // spdlog::info("y_max_min_diff: {}", y_max_min_diff);
            if(y_min_max_diff > 0 || y_max_min_diff < 0)
                continue;

            float y_offset = (std::abs(y_min_max_diff) > std::abs(y_max_min_diff)) ? 
                y_max_min_diff : y_min_max_diff;

            float z_min_max_diff =  c.min.z - extent.max.z;
            float z_max_min_diff =  c.max.z - extent.min.z;

            // spdlog::info("z_min_max_diff: {}", z_min_max_diff);
            // spdlog::info("z_max_min_diff: {}", z_max_min_diff);
            if(z_min_max_diff > 0 || z_max_min_diff < 0)
                continue;

            float z_offset = (std::abs(z_min_max_diff) > std::abs(z_max_min_diff)) ? 
                z_max_min_diff : z_min_max_diff;

            if(std::abs(x_offset) < std::abs(y_offset))
                if(std::abs(x_offset) < std::abs(z_offset)) {
                    body.velocity.x = 0;
                    offset.x = x_offset * 1.01;
                }
                else {
                    body.velocity.z = 0;
                    offset.z = z_offset * 1.01;
                }
            else
                if(std::abs(y_offset) < std::abs(z_offset)) {
                    if(body.grounded != Grounded::Locked) {
                        body.velocity.y = 0;
                        offset.y = y_offset * 1.01;
                    } 

                }
                else {
                    body.velocity.z = 0;
                    offset.z = z_offset * 1.01;
                }
        }
        return offset;
    }
    bool TestBoxCollision(const Extent3D& extent, const std::vector<Extent3D>& colliders) {
        for(auto& c : colliders) {
            float x_min_max_diff =  c.min.x - extent.max.x;
            float x_max_min_diff =  c.max.x - extent.min.x;

            if(x_min_max_diff > 0 || x_max_min_diff < 0)
                continue;

            float y_min_max_diff =  c.min.y - extent.max.y;
            float y_max_min_diff =  c.max.y - extent.min.y;

            if(y_min_max_diff > 0 || y_max_min_diff < 0)
                continue;

            float z_min_max_diff =  c.min.z - extent.max.z;
            float z_max_min_diff =  c.max.z - extent.min.z;

            if(z_min_max_diff > 0 || z_max_min_diff < 0)
                continue;

            return true;
        }
        return false;
    }
    glm::vec3 ApplyBoxCollisionResponseBounce(const Extent3D& extent, const glm::vec3& velocity, float bounciness, const std::vector<Extent3D>& colliders) {
        for(auto& c : colliders) {
            // spdlog::info("ITERATION::");
            float x_min_max_diff =  c.min.x - extent.max.x;
            float x_max_min_diff =  c.max.x - extent.min.x;
            // spdlog::info("x_min_max_diff: {}", x_min_max_diff);
            // spdlog::info("x_max_min_diff: {}", x_max_min_diff);

            if(x_min_max_diff > 0 || x_max_min_diff < 0)
                continue;

            float x_offset = (std::abs(x_min_max_diff) > std::abs(x_max_min_diff)) ? 
                x_max_min_diff : x_min_max_diff;

            float y_min_max_diff =  c.min.y - extent.max.y;
            float y_max_min_diff =  c.max.y - extent.min.y;

            // spdlog::info("y_min_max_diff: {}", y_min_max_diff);
            // spdlog::info("y_max_min_diff: {}", y_max_min_diff);
            if(y_min_max_diff > 0 || y_max_min_diff < 0)
                continue;

            float y_offset = (std::abs(y_min_max_diff) > std::abs(y_max_min_diff)) ? 
                y_max_min_diff : y_min_max_diff;

            float z_min_max_diff =  c.min.z - extent.max.z;
            float z_max_min_diff =  c.max.z - extent.min.z;

            // spdlog::info("z_min_max_diff: {}", z_min_max_diff);
            // spdlog::info("z_max_min_diff: {}", z_max_min_diff);
            if(z_min_max_diff > 0 || z_max_min_diff < 0)
                continue;

            float z_offset = (std::abs(z_min_max_diff) > std::abs(z_max_min_diff)) ? 
                z_max_min_diff : z_min_max_diff;

            if(std::abs(x_offset) < std::abs(y_offset))
                if(std::abs(x_offset) < std::abs(z_offset))
                    return glm::vec3(-velocity.x * bounciness, 0, 0);
                else
                    return glm::vec3(0, 0, -velocity.z * bounciness);

            else
                if(std::abs(y_offset) < std::abs(z_offset))
                    return glm::vec3(0, -velocity.y * bounciness, 0);
                else
                    return glm::vec3(0, 0, -velocity.z * bounciness);
        }
        return glm::vec3(0);
    }
}
