#include <spdlog/spdlog.h>
#include "player.h"
#include "block/block.h"
#include "input.h"
#include "physics/physics.h"

namespace Chemical {

    Player::Player(float speed, float vertical_speed, float sensitivity) :
        speed(speed), vertical_speed(vertical_speed), sensitivity(sensitivity) {
        Input::SubscribeToCursorPosCallback(Player::OnCursorPosCallback, this);
        Input::SubscribeToKeyCallback(Player::OnKeyCallback, this);

        // auto r = ApplyBoxCollisionResponse({{3, 0, 0}, {8, 0, 0}}, std::vector<Extent3D>
        //         {
        //          {{2, 0, 0}, {5 ,0 ,0}}, 
        //          });

        // spdlog::info("{}, {}, {}", r.x,r.y,r.z);
        other_colliders = GetTestColliderData();
        transform.position = glm::vec3(3, 25, 3);
    }
    Player::~Player() {
        Input::UnsubscribeFromCursorPosCallback(this);
        Input::UnsubscribeFromKeyCallback(this);
    }

    void Player::Update(wgpu::Queue queue, wgpu::Buffer view_buffer) {

        Extent3D collider = {
            glm::vec3(transform.position.x - 0.5, transform.position.y - 1, transform.position.z - 0.5),
            glm::vec3(transform.position.x + 0.5, transform.position.y, transform.position.z + 0.5)
        };
        auto offset = ApplyBoxCollisionResponse(collider, other_colliders);
        body.ApplyVelocityForce(offset);

        transform = body.ApplyVelocityToTransform(transform);

        if(!freecam) {
            body.ApplyGravity(0.000481f); // pcam only
            Extent3D ground_collider = {
                glm::vec3(transform.position.x - 0.4, transform.position.y - 1, transform.position.z - 0.4),
                glm::vec3(transform.position.x + 0.4, transform.position.y - 1.2, transform.position.z + 0.4)
            };
            is_grounded = TestBoxCollision(ground_collider, other_colliders);
            spdlog::info("IsGrounded: {}", is_grounded);
        }

        current_matrix = transform.AsMat4();
        if(direction_bitmask) {
            Move();
            current_matrix = transform.AsMat4();
        }


        queue.WriteBuffer(view_buffer, 0, &glm::inverse(current_matrix)[0][0], sizeof(glm::mat4));


    }
    void Player::OnKeyCallback(int key, int, int action, int, void* data) {
        Player& player = *reinterpret_cast<Player*>(data);

        if(key == GLFW_KEY_F && action == GLFW_PRESS)
            player.freecam = !player.freecam;

        static std::unordered_map<int, MoveDirection> key_to_direction = {
            {GLFW_KEY_W, MoveDirection::Forwards},
            {GLFW_KEY_S, MoveDirection::Backwards},
            {GLFW_KEY_D, MoveDirection::Right},
            {GLFW_KEY_A, MoveDirection::Left},
            {GLFW_KEY_SPACE, MoveDirection::Up},
            {GLFW_KEY_LEFT_CONTROL, MoveDirection::Down},
        };

        auto search = key_to_direction.find(key);
        if(search == key_to_direction.end())
            return;

        if(action == GLFW_REPEAT)
            return;

        player.direction_bitmask ^= (uint8_t)search->second;

    }

    void Player::OnCursorPosCallback(double xpos, double ypos, void* data) {
        Player& player = *reinterpret_cast<Player*>(data);

        player.transform.rotation.y += static_cast<float>(xpos - player.old_mouse_x) * player.sensitivity;
        player.transform.rotation.x += static_cast<float>(ypos - player.old_mouse_y) * player.sensitivity;

        player.transform.rotation.x = glm::clamp(player.transform.rotation.x, -85.0f, 85.0f);

        player.old_mouse_x = xpos;
        player.old_mouse_y = ypos;

        // camera.callback(glm::inverse(camera.transform.AsMat4()));
    }

    void Player::Move() {
        glm::fvec3 right = current_matrix[0];
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 forward = current_matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        // spdlog::info("fwd: {}, {}, {}", forward.x,forward.y,forward.z);
        if(direction_bitmask & (uint8_t)MoveDirection::Forwards)
            body.AddVelocity(front * speed);
        if(direction_bitmask & (uint8_t)MoveDirection::Backwards)
            body.AddVelocity(-front * speed);
        if(direction_bitmask & (uint8_t)MoveDirection::Right)
            body.AddVelocity(right * speed);
        if(direction_bitmask & (uint8_t)MoveDirection::Left)
            body.AddVelocity(-right * speed);
        if(freecam) {
            if(direction_bitmask & (uint8_t)MoveDirection::Up)
                body.AddVelocity(up * vertical_speed);
            if(direction_bitmask & (uint8_t)MoveDirection::Down)
                body.AddVelocity(-up * vertical_speed);
        } else {
            if(direction_bitmask & (uint8_t)MoveDirection::Up && is_grounded)
                body.ApplyVelocityForce(up * vertical_speed);

        }


    }
}
