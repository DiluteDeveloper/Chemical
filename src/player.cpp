#include <spdlog/spdlog.h>
#include "player.h"
#include <imgui.h>
#include "input.h"
#include "physics/physics.h"

namespace Chemical {

    Player::Player() {
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
    float mag = 0;

    // first = right, second = forward, third = -right, fourth = -forward
    int jumpdir = 0b00000000;
    bool jumping = false;
    void Player::Update(wgpu::Queue queue, wgpu::Buffer view_buffer, double _delta_time) {

        delta_time = _delta_time;


        // switch(body.grounded) {
        //     case Grounded::Locked:
        //         spdlog::info("LOCKED!"); break;
        //     case Grounded::NotGrounded:
        //         spdlog::info("NOT!"); break;
        //     case Grounded::Grounded:
        //         spdlog::info("GROUNDED!"); break;
        // }
        body.ApplyNaturalForces(delta_time); // pcam only
        if(body.grounded == Grounded::NotGrounded)
            body.ApplyGravity(delta_time); // pcam only
        Extent3D ground_collider = {
            glm::vec3(transform.position.x - 0.3, transform.position.y - 2, transform.position.z - 0.3),
            glm::vec3(transform.position.x + 0.3, transform.position.y - 2.02, transform.position.z + 0.3)
        };
        bool g = TestBoxCollision(ground_collider, other_colliders);


            
        Extent3D collider = {
            glm::vec3(transform.position.x - 0.5, transform.position.y - 1.5, transform.position.z - 0.5),
            glm::vec3(transform.position.x + 0.5, transform.position.y + 0.5, transform.position.z + 0.5)
        };
        auto offset = ApplyBoxCollisionResponse(body, collider, other_colliders);
        transform.position += offset;
        // body.ApplyVelocityForce(offset);


        // spdlog::info("Velocity 1: {:f}, {:f}, {:f}", body.velocity.x,body.velocity.y,body.velocity.z);
        if(body.grounded == Grounded::Grounded) {
            // spdlog::info("NOT GROUNDED");
            jumping = false;
            // mag = std::abs(body.velocity.x) + std::abs(body.velocity.z);
            // spdlog::info("MAG UPDATE");
        }

        if(body.grounded == Grounded::Locked)  {
            if(!g) {
                body.grounded = Grounded::NotGrounded;
            }
        } else {
            body.grounded = (Grounded)g;
        }
        current_matrix = transform.AsMat4();
        // spdlog::info("Velocity 2: {:f}, {:f}, {:f}", body.velocity.x,body.velocity.y,body.velocity.z);
        Move();
        current_matrix = transform.AsMat4();

        transform = body.ApplyVelocityToTransform(transform);

        queue.WriteBuffer(view_buffer, 0, &glm::inverse(current_matrix)[0][0], sizeof(glm::mat4));



        ConfigurationWindow();
    }
    void Player::ConfigurationWindow() {
        ImGui::Begin("Player Configuration");

        ImGui::InputFloat("Set Speed", &speed, 0, 5, "%.5f");
         ImGui::InputFloat("Set Gravity", &body.gravity, 0, 5, "%.5f");
        ImGui::InputFloat("Set Jump Force", &jump_force, 0, 50, "%.5f");
         ImGui::InputFloat("Set Friction", &body.friction, 0, 1, "%.5f");
         // ImGui::InputFloat("Set Air Speed Increase", &body.air_speed_increase, 0, 2, "%.5f");
        //ImGui::SliderFloat3("Set Terminal Velocity", (float*)&body.terminal_velocity, 0, 5, "%.3f");

        // static bool on_vsync = false;
        // if(ImGui::Button(std::format("Toggle vsync (vsync is {}))", on_vsync).c_str())) {
        //     on_vsync = !on_vsync;
        //     glfwSwapInterval(on_vsync);
        // }
        ImGui::End();
    }

    void Player::OnKeyCallback(int key, int, int action, int, void* data) {
        Player& player = *reinterpret_cast<Player*>(data);

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            if(player.input_enabled)
                Input::SetCursorEnabled();
            else
                Input::SetCursorDisabled();
            player.input_enabled = !player.input_enabled;
        }
        if(!player.input_enabled)
            return;

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
        if(!player.input_enabled)
            return;


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


        if(freecam) {
            if(direction_bitmask & (uint8_t)MoveDirection::Up)
                body.AddVelocity(up * vertical_speed * (float)delta_time);
            if(direction_bitmask & (uint8_t)MoveDirection::Down)
                body.AddVelocity(-up * vertical_speed * (float)delta_time);
        } else {
            if(direction_bitmask & (uint8_t)MoveDirection::Up && body.grounded == Grounded::Grounded) {
                // spdlog::info("JUMP!");
                body.ApplyVelocityForce(up * jump_force * (float)delta_time);
                body.grounded = Grounded::Locked;
                jumping = true;
                jumpdir = 0;
                if(direction_bitmask & (uint8_t)MoveDirection::Forwards)
                   jumpdir ^= 0b0100;
                if(direction_bitmask & (uint8_t)MoveDirection::Backwards)
                   jumpdir ^= 0b0001;
                if(direction_bitmask & (uint8_t)MoveDirection::Right)
                   jumpdir ^= 0b1000;
                if(direction_bitmask & (uint8_t)MoveDirection::Left)
                   jumpdir ^= 0b0010;
                if(!jumpdir)
                    jumping = false;
                spdlog::info("Jumped, jumping={}, mask={:b}, dirmask={:b}", jumping, jumpdir, direction_bitmask);
                mag = std::abs(body.velocity.x) + std::abs(body.velocity.z);
            }


        }
        // spdlog::info("MAG: {}", mag);
        // spdlog::info("jumpdir: {}", jumpdir);

        glm::fvec3 vel = glm::fvec3(0);
        if(jumping) {
            if(jumpdir & 0b1000)
                vel += right;
            if(jumpdir & 0b0100)
                vel += front;
            if(jumpdir & 0b0010)
                vel += -right;
            if(jumpdir & 0b0001)
                vel += -front;
            vel.x = std::min(vel.x, 1.0f);
            vel.x = std::max(vel.x, -1.0f);
            vel.z = std::min(vel.z, 1.0f);
            vel.z = std::max(vel.z, -1.0f);
            body.velocity = glm::vec3(mag * vel.x, body.velocity.y, mag * vel.z);
            return;
        }

        if(direction_bitmask & (uint8_t)MoveDirection::Forwards)
            vel += front;
        if(direction_bitmask & (uint8_t)MoveDirection::Backwards)
            vel += -front;
        if(direction_bitmask & (uint8_t)MoveDirection::Right)
            vel += right;
        if(direction_bitmask & (uint8_t)MoveDirection::Left)
            vel += -right;
        vel.x = std::min(vel.x, 1.0f);
        vel.x = std::max(vel.x, -1.0f);
        vel.z = std::min(vel.z, 1.0f);
        vel.z = std::max(vel.z, -1.0f);
        if(body.grounded != Grounded::Grounded) {
            if(std::abs(body.velocity.x + (vel.x* speed * (float)delta_time)) > std::abs(body.velocity.x))
                vel.x = 0;
            if(std::abs(body.velocity.z + (vel.z * speed * (float)delta_time)) > std::abs(body.velocity.z))
                vel.z = 0;
        }
        body.AddVelocity(vel * speed * (float)delta_time);

    }
}
