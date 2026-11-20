#include <imgui.h>
#include "input.h"
#include "components.h"
#include "spdlog/spdlog.h"

namespace Chemical {

namespace {


    glm::dvec2 mouse_offset_since_last_update = glm::vec2(0.0f);
    glm::dvec2 last_mouse_pos = glm::vec2(0.0f);
    bool input_enabled = true;
    bool* freecam_enabled = nullptr;
    uint32_t direction_flags = 0;

    void OnKeyCallback(int key, int, int action, int, void*) {
        if (action == GLFW_PRESS) {
            switch(key) {
                case GLFW_KEY_ESCAPE:
                    if(input_enabled)
                        Input::SetCursorEnabled();
                    else
                        Input::SetCursorDisabled();
                    input_enabled = !input_enabled;
                    break;
                case GLFW_KEY_F:
                    *freecam_enabled = !(*freecam_enabled);
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

        direction_flags ^= (uint8_t)search->second;

    }

    void OnCursorPosCallback(double xpos, double ypos, void*) {
        if(!input_enabled)
            return;

        mouse_offset_since_last_update.x += xpos - last_mouse_pos.x;
        mouse_offset_since_last_update.y += ypos - last_mouse_pos.y;
        // body.transform.rotation.y += static_cast<float>(xpos - last_mouse_pos.x) * CharacterController::sensitivity;
        // body.transform.rotation.x += static_cast<float>(ypos - last_mouse_pos.y) * CharacterController::sensitivity;

        // body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        last_mouse_pos.x = xpos;
        last_mouse_pos.y = ypos;
    }

    void UpdateFreecamController(PhysicsBodyComponent& body, CharacterControllerComponent& controller, BoxColliders::ConstView colliders, double delta_time, wgpu::Queue queue, wgpu::Buffer buffer) {

        body.transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * controller.sensitivity;
        body.transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * controller.sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        glm::mat4 matrix = body.transform.AsMat4();
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 right = matrix[0];
        controller.forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(controller.forward.x, 0, controller.forward.z));

        glm::fvec3 velocity = glm::fvec3(0);
        if(direction_flags & (uint8_t)Direction::Forwards)
            velocity = front;
        if(direction_flags & (uint8_t)Direction::Backwards)
            velocity += -front;
        if(direction_flags & (uint8_t)Direction::Right)
            velocity += right;
        if(direction_flags & (uint8_t)Direction::Left)
            velocity += -right;
        if(direction_flags & (uint8_t)Direction::Up)
            velocity += up;
        if(direction_flags & (uint8_t)Direction::Down)
            velocity += -up;

        velocity.x = std::min(velocity.x, 1.0f);
        velocity.x = std::max(velocity.x, -1.0f);
        velocity.y = std::min(velocity.y, 1.0f);
        velocity.y = std::max(velocity.y, -1.0f);
        velocity.z = std::min(velocity.z, 1.0f);
        velocity.z = std::max(velocity.z, -1.0f);



        body.velocity += velocity * controller.base_speed * (float)delta_time;

        body.velocity *= glm::fvec3(1.0f) - (controller.freecam_friction * (float)delta_time);

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Left)
            body.velocity.x = std::max(0.0f, body.velocity.x);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Right)
            body.velocity.x = std::min(0.0f, body.velocity.x);

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Down)
            body.velocity.y = std::max(0.0f, body.velocity.y);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Up)
            body.velocity.y = std::min(0.0f, body.velocity.y);

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Backwards)
            body.velocity.z = std::max(0.0f, body.velocity.z);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Forwards)
            body.velocity.z = std::min(0.0f, body.velocity.z);

        body.transform.position += body.velocity * (float)delta_time;

        controller.delta_accumulator += delta_time;
        if(controller.delta_accumulator >= 1.0f / controller.PHYSICS_FRAME_RATE) {
            CollisionResponse response = TriggerBoxColliders::GetCollisionResponse(TriggerBoxColliderComponent{
                body.transform.position + controller.main_collider_offset.min,
                body.transform.position + controller.main_collider_offset.max
            }, colliders);

            body.transform.position += response.offset;
            controller.last_collision_direction_flags = response.direction_flags;
            controller.delta_accumulator = 0.0f;
        }

        queue.WriteBuffer(buffer, 0, &glm::inverse(body.transform.AsMat4())[0][0], sizeof(glm::mat4));
    }
    void UpdateCharacterController(PhysicsBodyComponent& body, CharacterControllerComponent& controller, BoxColliders::ConstView colliders, double delta_time, wgpu::Queue queue, wgpu::Buffer buffer) {

        body.transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * controller.sensitivity;
        body.transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * controller.sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        glm::mat4 matrix = body.transform.AsMat4();
        glm::fvec3 right = matrix[0];
        controller.forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(controller.forward.x, 0, controller.forward.z));

        glm::fvec3 velocity = glm::fvec3(0);
        if(direction_flags & (uint8_t)Direction::Forwards)
            velocity = front;
        if(direction_flags & (uint8_t)Direction::Backwards)
            velocity += -front;
        if(direction_flags & (uint8_t)Direction::Right)
            velocity += right;
        if(direction_flags & (uint8_t)Direction::Left)
            velocity += -right;
        if(direction_flags & (uint8_t)Direction::Up && controller.is_grounded)
            body.velocity.y = controller.jump_force;

        velocity.x = std::min(velocity.x, 1.0f);
        velocity.x = std::max(velocity.x, -1.0f);
        velocity.z = std::min(velocity.z, 1.0f);
        velocity.z = std::max(velocity.z, -1.0f);

        body.velocity += velocity * (controller.base_speed + controller.bhop_speed) * (float)delta_time;

        body.velocity *= glm::fvec3(1.0f) - (controller.controller_friction * (float)delta_time);
        body.velocity.y -= controller.gravity * delta_time; 

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Left)
            body.velocity.x = std::max(0.0f, body.velocity.x);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Right)
            body.velocity.x = std::min(0.0f, body.velocity.x);

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Down)
            body.velocity.y = std::max(0.0f, body.velocity.y);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Up)
            body.velocity.y = std::min(0.0f, body.velocity.y);

        if(controller.last_collision_direction_flags & (uint8_t)Direction::Backwards)
            body.velocity.z = std::max(0.0f, body.velocity.z);
        if(controller.last_collision_direction_flags & (uint8_t)Direction::Forwards)
            body.velocity.z = std::min(0.0f, body.velocity.z);


        body.transform.position += body.velocity * (float)delta_time;
        controller.delta_accumulator += delta_time;
        if(controller.delta_accumulator >= 1.0f / controller.PHYSICS_FRAME_RATE) {
            CollisionResponse response = TriggerBoxColliders::GetCollisionResponse(TriggerBoxColliderComponent{
                body.transform.position + controller.main_collider_offset.min,
                body.transform.position + controller.main_collider_offset.max
            }, colliders);
            controller.is_grounded = TriggerBoxColliders::IsColliding(TriggerBoxColliderComponent{
                body.transform.position + controller.ground_collider_offset.min,
                body.transform.position + controller.ground_collider_offset.max
            }, colliders);

            body.transform.position += response.offset;
            controller.last_collision_direction_flags = response.direction_flags;
            controller.delta_accumulator = 0.0f;
        }

        if(controller.last_collision_direction_flags & 0b11111111)
            controller.bhop_speed = std::max(0.0, controller.bhop_speed - (controller.bhop_speed * controller.bhop_speed_decrease_rate * delta_time));
        else
            controller.bhop_speed += controller.bhop_speed_increase_rate * delta_time;

        queue.WriteBuffer(buffer, 0, &glm::inverse(body.transform.AsMat4())[0][0], sizeof(glm::mat4));
    }
}
    void CharacterControllerComponent::on_construct(entt::registry&, const entt::entity) {

        Input::SubscribeToCursorPosCallback(OnCursorPosCallback, nullptr);
        Input::SubscribeToKeyCallback(OnKeyCallback, nullptr);
    }

    void CharacterControllers::Update(View con, BoxColliders::ConstView colliders, double delta_time, wgpu::Queue queue, wgpu::Buffer buffer) {

        for(auto [e, body, controller] : con.each()) {
            if(controller.freecam_enabled)
                UpdateFreecamController(body, controller, colliders, delta_time, queue, buffer);
            else
                UpdateCharacterController(body, controller, colliders, delta_time, queue, buffer);
            //
            // ImGui::Begin("Player Configuration");
            //
            // ImGui::InputFloat("Set Speed", &controller.base_speed, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Bhop Speed Increase Rate", &controller.bhop_speed_increase_rate, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Bhop Speed Decrease Rate", &controller.bhop_speed_decrease_rate, 0, 5, "%.5f");
            // ImGui::Text("Bhop speed: %.2f", controller.bhop_speed);
            // ImGui::InputFloat("Set Jump Force", &controller.jump_force, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Gravity", &controller.gravity, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Sensitivity", &controller.sensitivity, 0, 5, "%.5f");
            // // ImGui::InputFloat("Set Jump Force", &jump_force, 0, 50, "%.5f");
            // ImGui::InputFloat3("Set Controller Friction", &controller.controller_friction[0], 0, 1);
            // ImGui::InputFloat3("Set Freecam Friction", &controller.freecam_friction[0], 0, 1);
            // if(ImGui::Button("Toggle Freecam")) {
            //     controller.freecam_enabled = !controller.freecam_enabled;
            // }
            // ImGui::End();
        }

    }


    void CharacterControllers::AddTestControllerToRegistry(entt::registry& r) {
        entt::entity entity = r.create();
        r.emplace<PhysicsBodyComponent>(entity, PhysicsBodyComponent{
            .transform = { .position = {0, 5, 0}}});
        auto& c = r.emplace<CharacterControllerComponent>(entity, CharacterControllerComponent{
            .base_speed = 80.0f,
            .bhop_speed_decrease_rate = 8.0f,
            .bhop_speed_increase_rate = 13.0f,
            .jump_force = 12.2f,
#ifdef WSL
            .sensitivity = 0.5,
#else
            .sensitivity = 0.1,
#endif
            .controller_friction = glm::fvec3(10.0f, 0.0f, 10.00f),
            .freecam_friction = glm::fvec3(8.0f),
            .gravity = 35.00f,
            .main_collider_offset = {{-0.5f, -2.0f, -0.5f}, {0.5f, 0.5f, 0.5f}},
            .ground_collider_offset = {{-0.2f, -2.2f, -0.2f}, {0.2f, -2.0f, 0.2f}}
            });
        freecam_enabled = &c.freecam_enabled;
    }
}
