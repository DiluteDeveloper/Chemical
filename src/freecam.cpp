#include "freecam.h"

#include "input.h"
#include "physics/physics.h"

namespace Chemical {

    enum class MoveDirection:uint8_t {
        None = 0b00000000, 
        Up = 0b00000001, 
        Down = 0b00000010, 
        Left = 0b00000100, 
        Right = 0b00001000, 
        Forwards = 0b00010000, 
        Backwards = 0b00100000
    };
    uint8_t direction_flags = 0;

    float Freecam::speed = 0.31f;
#ifdef WSL
    float Freecam::sensitivity = 0.5f;
#else
    float Freecam::sensitivity = 0.2f;
#endif

    bool input_enabled = true;

    entt::entity entity;
    glm::mat4 matrix = glm::mat4(1.0f);

    glm::dvec2 mouse_offset_since_last_update = glm::vec2(0.0f);
    glm::dvec2 last_mouse_pos = glm::vec2(0.0f);

    void OnKeyCallback(int key, int, int action, int, void*) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            if(input_enabled)
                Input::SetCursorEnabled();
            else
                Input::SetCursorDisabled();
            input_enabled = !input_enabled;
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
        // body.transform.rotation.y += static_cast<float>(xpos - last_mouse_pos.x) * Freecam::sensitivity;
        // body.transform.rotation.x += static_cast<float>(ypos - last_mouse_pos.y) * Freecam::sensitivity;

        // body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        last_mouse_pos.x = xpos;
        last_mouse_pos.y = ypos;
    }

    void Freecam::Initialise(entt::registry& registry) {
        entity = registry.create();
        registry.emplace<PhysicsBodyComponent>(entity, PhysicsBodyComponent{
            .transform = { .position = {0, 5, 0}},
            .friction = 0.97f,
            .collider_min_extent = {-0.5f, -2.0f, -0.5f},
            .collider_max_extent = {0.5f, 0.5f, 0.5f}
        });

        Input::SubscribeToCursorPosCallback(OnCursorPosCallback, nullptr);
        Input::SubscribeToKeyCallback(OnKeyCallback, nullptr);
    }

    void Freecam::Update(entt::registry& registry, double delta_time) {
        auto& body = registry.get<PhysicsBodyComponent>(entity);

        body.transform.rotation.y += static_cast<float>(mouse_offset_since_last_update.x) * Freecam::sensitivity;
        body.transform.rotation.x += static_cast<float>(mouse_offset_since_last_update.y) * Freecam::sensitivity;
        mouse_offset_since_last_update = glm::dvec2(0);

        body.transform.rotation.x = glm::clamp(body.transform.rotation.x, -85.0f, 85.0f);

        matrix = body.transform.AsMat4();

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

        body.velocity += velocity * speed * (float)delta_time;
    }
    void Freecam::WriteViewMatrix(wgpu::Queue queue, wgpu::Buffer view_buffer) {
        queue.WriteBuffer(view_buffer, 0, &glm::inverse(matrix)[0][0], sizeof(glm::mat4));
    }
}
