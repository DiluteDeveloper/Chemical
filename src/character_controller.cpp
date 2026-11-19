#include <imgui.h>
#include "input.h"
#include "components.h"

namespace Chemical {

namespace {

    enum class MoveDirection:uint8_t {
        None = 0b00000000, 
        Up = 0b00000001, 
        Down = 0b00000010, 
        Left = 0b00000100, 
        Right = 0b00001000, 
        Forwards = 0b00010000, 
        Backwards = 0b00100000
    };
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
        glm::fvec3 right = matrix[0];
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        glm::fvec3 velocity = glm::fvec3(0);
        if(direction_flags & (uint8_t)MoveDirection::Forwards)
            velocity = front;
        if(direction_flags & (uint8_t)MoveDirection::Backwards)
            velocity += -front;
        if(direction_flags & (uint8_t)MoveDirection::Right)
            velocity += right;
        if(direction_flags & (uint8_t)MoveDirection::Left)
            velocity += -right;
        if(direction_flags & (uint8_t)MoveDirection::Up)
            velocity += up;
        if(direction_flags & (uint8_t)MoveDirection::Down)
            velocity += -up;

        velocity.x = std::min(velocity.x, 1.0f);
        velocity.x = std::max(velocity.x, -1.0f);
        velocity.y = std::min(velocity.y, 1.0f);
        velocity.y = std::max(velocity.y, -1.0f);
        velocity.z = std::min(velocity.z, 1.0f);
        velocity.z = std::max(velocity.z, -1.0f);

        body.velocity += velocity * controller.speed * (float)delta_time;

        body.velocity *= glm::fvec3(1.0f) - controller.freecam_friction; 

        glm::fvec3 offset = TriggerBoxColliders::GetCollisionResponse(TriggerBoxColliderComponent{
            body.transform.position + controller.main_collider_offset.min,
            body.transform.position + controller.main_collider_offset.max
        }, colliders);

        body.transform.position += offset;
        if(offset.x != 0)
            body.velocity.x = 0;
        if(offset.y != 0)
            body.velocity.y = 0;
        if(offset.z != 0)
            body.velocity.z = 0;
        body.transform.position += body.velocity;

        queue.WriteBuffer(buffer, 0, &glm::inverse(body.transform.AsMat4())[0][0], sizeof(glm::mat4));
    }
    void UpdateCharacterController(PhysicsBodyComponent& body, CharacterControllerComponent& controller, BoxColliders::ConstView colliders, double delta_time, wgpu::Queue queue, wgpu::Buffer buffer) {

        body.transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * controller.sensitivity;
        body.transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * controller.sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        glm::mat4 matrix = body.transform.AsMat4();
        glm::fvec3 right = matrix[0];
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        glm::fvec3 velocity = glm::fvec3(0);
        if(direction_flags & (uint8_t)MoveDirection::Forwards)
            velocity = front;
        if(direction_flags & (uint8_t)MoveDirection::Backwards)
            velocity += -front;
        if(direction_flags & (uint8_t)MoveDirection::Right)
            velocity += right;
        if(direction_flags & (uint8_t)MoveDirection::Left)
            velocity += -right;
        if(direction_flags & (uint8_t)MoveDirection::Up)
            velocity += up;
        if(direction_flags & (uint8_t)MoveDirection::Down)
            velocity += -up;

        velocity.x = std::min(velocity.x, 1.0f);
        velocity.x = std::max(velocity.x, -1.0f);
        velocity.y = std::min(velocity.y, 1.0f);
        velocity.y = std::max(velocity.y, -1.0f);
        velocity.z = std::min(velocity.z, 1.0f);
        velocity.z = std::max(velocity.z, -1.0f);

        body.velocity += velocity * controller.speed * (float)delta_time;

        body.velocity *= glm::fvec3(1.0f) - controller.controller_friction;
        body.velocity.y -= controller.gravity; 

        body.transform.position += body.velocity;
        glm::fvec3 offset = TriggerBoxColliders::GetCollisionResponse(TriggerBoxColliderComponent{
            body.transform.position + controller.main_collider_offset.min,
            body.transform.position + controller.main_collider_offset.max
        }, colliders);

        body.transform.position += offset;
        if(offset.x != 0)
            body.velocity.x = 0;
        if(offset.y != 0)
            body.velocity.y = 0;
        if(offset.z != 0)
            body.velocity.z = 0;

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

            ImGui::Begin("Player Configuration");

            ImGui::InputFloat("Set Speed", &controller.speed, 0, 5, "%.5f");
            ImGui::InputFloat("Set Gravity", &controller.gravity, 0, 5, "%.5f");
            ImGui::InputFloat("Set Sensitivity", &controller.sensitivity, 0, 5, "%.5f");
            // ImGui::InputFloat("Set Jump Force", &jump_force, 0, 50, "%.5f");
            ImGui::InputFloat3("Set Controller Friction", &controller.controller_friction[0], 0, 1);
            ImGui::InputFloat3("Set Freecam Friction", &controller.freecam_friction[0], 0, 1);
            if(ImGui::Button("Toggle Freecam")) {
                controller.freecam_enabled = !controller.freecam_enabled;
            }
            ImGui::End();
        }

    }


    void CharacterControllers::AddTestControllerToRegistry(entt::registry& r) {
        entt::entity entity = r.create();
        r.emplace<PhysicsBodyComponent>(entity, PhysicsBodyComponent{
            .transform = { .position = {0, 5, 0}}});
        auto& c = r.emplace<CharacterControllerComponent>(entity, CharacterControllerComponent{
            .speed = 3.0f,
            .controller_friction = glm::fvec3(0.11f, 0.0f, 0.11f),
            .freecam_friction = glm::fvec3(0.11f),
            .gravity = 0.11f,
            .main_collider_offset = {{-0.5f, -2.0f, -0.5f}, {0.5, 0.5, 0.5}},
            .ground_collider_offset = {{-0.5f, -2.0f, -0.5f}, {0.5, 0.5, 0.5}}
            });
        freecam_enabled = &c.freecam_enabled;
    }
    // void CharacterController::Initialise(entt::registry& registry) {
    //     entity = registry.create();
    //     registry.emplace<PhysicsBodyComponent>(entity, PhysicsBodyComponent{
    //         .transform = { .position = {0, 5, 0}},
    //         .friction = glm::fvec3(0.05f, 0.0f, 0.05f),
    //         .collider_min_extent = {-0.5f, -2.0f, -0.5f},
    //         .collider_max_extent = {0.5f, 0.5f, 0.5f}
    //     });
    //
    //     Input::SubscribeToCursorPosCallback(OnCursorPosCallback, nullptr);
    //     Input::SubscribeToKeyCallback(OnKeyCallback, nullptr);
    // }

    // void CharacterController::Update(entt::registry& registry, double delta_time) {
    //     auto& body = registry.get<PhysicsBodyComponent>(entity);
    //
    //     body.transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * CharacterController::sensitivity;
    //     body.transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * CharacterController::sensitivity;
    //     mouse_offset_since_last_update = glm::dvec2(0);
    //
    //     body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);
    //
    //     matrix = body.transform.AsMat4();
    //
    //     glm::fvec3 right = matrix[0];
    //     glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
    //     glm::fvec3 forward = matrix[2];
    //     glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));
    //
    //     if(direction_flags & (uint8_t)MoveDirection::Up && body.grounded == Grounded::Grounded) {
    //
    //     // If the player jumps forwards, 
    //     // velocity is set to the forwards direction multiplied by the magnitude
    //     // of the velocity last frame. If the player is in the air after jumping
    //     // in a direction that is not forward or after being in the air for any other reason,
    //     // friction is normal.
    //     // Add velocity in the direction you are going all the time with friction,
    //     // the only exception is that when you are in the air going forwards,
    //     // you gain momentum instead of experiencing friction
    //
    //     // glm::fvec3 velocity = glm::fvec3(0);
    //     // if(direction_flags & (uint8_t)MoveDirection::Forwards)
    //     //     velocity = front;
    //     // if(direction_flags & (uint8_t)MoveDirection::Backwards)
    //     //     velocity += -front;
    //     // if(direction_flags & (uint8_t)MoveDirection::Right)
    //     //     velocity += right;
    //     // if(direction_flags & (uint8_t)MoveDirection::Left)
    //     //     velocity += -right;
    //     // if(direction_flags & (uint8_t)MoveDirection::Up)
    //     //     velocity += up;
    //     // if(direction_flags & (uint8_t)MoveDirection::Down)
    //     //     velocity += -up;
    //     //
    //     // velocity.x = std::min(velocity.x, 1.0f);
    //     // velocity.x = std::max(velocity.x, -1.0f);
    //     // velocity.y = std::min(velocity.y, 1.0f);
    //     // velocity.y = std::max(velocity.y, -1.0f);
    //     // velocity.z = std::min(velocity.z, 1.0f);
    //     // velocity.z = std::max(velocity.z, -1.0f);
    //     //
    //     // body.velocity += velocity * speed * (float)delta_time;
    // }
    // void CharacterController::WriteViewMatrix(wgpu::Queue queue, wgpu::Buffer view_buffer) {
    //     queue.WriteBuffer(view_buffer, 0, &glm::inverse(matrix)[0][0], sizeof(glm::mat4));
    // }
}
