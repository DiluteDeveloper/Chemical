#include "input.h"
#include <spdlog/spdlog.h>
#include "character_controller.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Chemical {

    void CharacterController::OnKeyCallback(int key, int, int action, int) {
        if (action == GLFW_PRESS) {
            switch(key) {
                case GLFW_KEY_ESCAPE:
                    if(input_enabled)
                        input->SetCursorEnabled();
                    else
                        input->SetCursorDisabled();
                    input_enabled = !input_enabled;
                    break;
                case GLFW_KEY_F:
                    freecam_enabled = !freecam_enabled;
                    break;
            }
        }

        if(!input_enabled)
            return;

        static std::unordered_map<int, Direction> key_to_direction = {
            {GLFW_KEY_W, Direction::Forwards},
            {GLFW_KEY_S, Direction::Backwards},
            {GLFW_KEY_D, Direction::Right},
            {GLFW_KEY_A, Direction::Left},
            {GLFW_KEY_SPACE, Direction::Up},
            {GLFW_KEY_LEFT_CONTROL, Direction::Down},
        };

        auto search = key_to_direction.find(key);
        if(search == key_to_direction.end())
            return;

        if(action == GLFW_REPEAT)
            return;

        direction_flags.flip((uint8_t)search->second);

    }

    void CharacterController::OnCursorPosCallback(double xpos, double ypos) {
        if(!input_enabled)
            return;

        mouse_offset_since_last_update.x += xpos - last_mouse_pos.x;
        mouse_offset_since_last_update.y += ypos - last_mouse_pos.y;

        last_mouse_pos.x = xpos;
        last_mouse_pos.y = ypos;
    }

    void CharacterController::UpdateFreecamController(BoxColliderConstView colliders, double delta_time) {

        transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * sensitivity;
        transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

        glm::mat4 matrix = transform.AsMat4();
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 right = matrix[0];
        forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        glm::fvec3 new_velocity = glm::fvec3(0);
        if(direction_flags.test((uint8_t)(uint8_t)Direction::Forwards))
            new_velocity = front;
        if(direction_flags.test((uint8_t)Direction::Backwards))
            new_velocity += -front;
        if(direction_flags.test((uint8_t)Direction::Right))
            new_velocity += right;
        if(direction_flags.test((uint8_t)Direction::Left))
            new_velocity += -right;
        if(direction_flags.test((uint8_t)Direction::Up))
            new_velocity += up;
        if(direction_flags.test((uint8_t)Direction::Down))
            new_velocity += -up;

        new_velocity.x = std::min(new_velocity.x, 1.0f);
        new_velocity.x = std::max(new_velocity.x, -1.0f);
        new_velocity.y = std::min(new_velocity.y, 1.0f);
        new_velocity.y = std::max(new_velocity.y, -1.0f);
        new_velocity.z = std::min(new_velocity.z, 1.0f);
        new_velocity.z = std::max(new_velocity.z, -1.0f);



        velocity += new_velocity * base_speed * (float)delta_time;

        velocity *= glm::fvec3(1.0f) - (freecam_friction * (float)delta_time);

        if(last_collision_direction_flags.test((uint8_t)Direction::Left))
            velocity.x = std::max(0.0f, velocity.x);
        if(last_collision_direction_flags.test((uint8_t)Direction::Right))
            velocity.x = std::min(0.0f, velocity.x);

        if(last_collision_direction_flags.test((uint8_t)Direction::Down))
            velocity.y = std::max(0.0f, velocity.y);
        if(last_collision_direction_flags.test((uint8_t)Direction::Up))
            velocity.y = std::min(0.0f, velocity.y);

        if(last_collision_direction_flags.test((uint8_t)Direction::Backwards))
            velocity.z = std::max(0.0f, velocity.z);
        if(last_collision_direction_flags.test((uint8_t)Direction::Forwards))
            velocity.z = std::min(0.0f, velocity.z);

        transform.position += velocity * (float)delta_time;

        delta_accumulator += delta_time;
        if(delta_accumulator >= 1.0f / PHYSICS_FRAME_RATE) {
            CollisionResponse response = TriggerBoxCollider::GetCollisionResponse(TriggerBoxCollider{
                transform.position + main_collider_offset.min,
                transform.position + main_collider_offset.max
            }, colliders);

            transform.position += response.offset;
            last_collision_direction_flags = response.direction_flags;
            delta_accumulator = 0.0f;
        }

        if(view_matrix_update_callback) view_matrix_update_callback(glm::inverse(transform.AsMat4()));
    }
    void CharacterController::UpdateCharacterController(BoxColliderConstView colliders, double delta_time) {

        transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * sensitivity;
        transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

        glm::mat4 matrix = transform.AsMat4();
        glm::fvec3 right = matrix[0];
        forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        glm::fvec3 new_velocity = glm::fvec3(0);
        if(direction_flags.test((uint8_t)Direction::Forwards))
            new_velocity = front;
        if(direction_flags.test((uint8_t)Direction::Backwards))
            new_velocity += -front;
        if(direction_flags.test((uint8_t)Direction::Right))
            new_velocity += right;
        if(direction_flags.test((uint8_t)Direction::Left))
            new_velocity += -right;
        if(direction_flags.test((uint8_t)Direction::Up) && is_grounded)
            velocity.y = jump_force;

        new_velocity.x = std::min(new_velocity.x, 1.0f);
        new_velocity.x = std::max(new_velocity.x, -1.0f);
        new_velocity.z = std::min(new_velocity.z, 1.0f);
        new_velocity.z = std::max(new_velocity.z, -1.0f);

        velocity += new_velocity * (base_speed + bhop_speed) * (float)delta_time;

        velocity *= glm::fvec3(1.0f) - (controller_friction * (float)delta_time);
        velocity.y -= gravity * delta_time; 

        if(last_collision_direction_flags.test((uint8_t)Direction::Left)) 
            velocity.x = std::max(0.0f, velocity.x);
        if(last_collision_direction_flags.test((uint8_t)Direction::Right))
            velocity.x = std::min(0.0f, velocity.x);

        if(last_collision_direction_flags.test((uint8_t)Direction::Down))
            velocity.y = std::max(0.0f, velocity.y);
        if(last_collision_direction_flags.test((uint8_t)Direction::Up))
            velocity.y = std::min(0.0f, velocity.y);

        if(last_collision_direction_flags.test((uint8_t)Direction::Backwards))
            velocity.z = std::max(0.0f, velocity.z);
        if(last_collision_direction_flags.test((uint8_t)Direction::Forwards))
            velocity.z = std::min(0.0f, velocity.z);

        transform.position += velocity * (float)delta_time;
        delta_accumulator += delta_time;
        if(delta_accumulator >= 1.0f / PHYSICS_FRAME_RATE) {
            CollisionResponse response = TriggerBoxCollider::GetCollisionResponse(TriggerBoxCollider{
                transform.position + main_collider_offset.min,
                transform.position + main_collider_offset.max
            }, colliders);
            is_grounded = TriggerBoxCollider::IsColliding(TriggerBoxCollider{
                transform.position + ground_collider_offset.min,
                transform.position + ground_collider_offset.max
            }, colliders);

            transform.position += response.offset;
            last_collision_direction_flags = response.direction_flags;
            delta_accumulator = 0.0f;
        }

        if(last_collision_direction_flags.any() || (direction_flags & MOVEMENT_DIRECTIONS).none())
            bhop_speed = std::max(0.0, bhop_speed - (bhop_speed * bhop_speed_decrease_rate * delta_time));
        else
            bhop_speed += bhop_speed_increase_rate * delta_time;

        if(view_matrix_update_callback) view_matrix_update_callback(glm::inverse(transform.AsMat4()));
    }

    CharacterController::CharacterController(Input& input) : input(&input) {
        input.SubscribeToCursorPosCallback(std::bind_front(&CharacterController::OnCursorPosCallback, this), this);
        input.SubscribeToKeyCallback(std::bind_front(&CharacterController::OnKeyCallback, this), this);
    }
    CharacterController::~CharacterController() {
        input->UnsubscribeFromCursorPosCallback(this);
        input->UnsubscribeFromKeyCallback(this);
    }
    void CharacterController::SetViewMatrixUpdateCallback(std::function<void(const glm::mat4& view_matrix)> cb) {
        view_matrix_update_callback = cb;
    }

    void CharacterController::Update(BoxColliderConstView colliders, double delta_time) {

            if(freecam_enabled)
                UpdateFreecamController(colliders, delta_time);
            else
                UpdateCharacterController(colliders, delta_time);

    }
}
