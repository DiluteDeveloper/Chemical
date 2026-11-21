#pragma once

#include "input.h"
#include "physics.h"
#include "utils/transform.h"
#include <bitset>
#include <webgpu/webgpu_cpp.h>

namespace Chemical {

    struct CharacterController {

        using BoxColliderConstView = entt::view<entt::get_t<const BoxCollider>>;

        CharacterController(Input& input);
        ~CharacterController();
        void Update(BoxColliderConstView colliders, double delta_time);
        void SetViewMatrixUpdateCallback(std::function<void(const glm::mat4& view_matrix)>);

        static constexpr uint32_t PHYSICS_FRAME_RATE = 60;
        static constexpr std::bitset<8> MOVEMENT_DIRECTIONS = std::bitset<8>().set((uint8_t)Direction::Forwards).set((uint8_t)Direction::Backwards).set((uint8_t)Direction::Left).set((uint8_t)Direction::Right);

        Transform transform = { .position = {0.0f, 20.0f, 0.0f}};
        glm::fvec3 velocity = glm::fvec3(0);

        float base_speed = 80.0f;
        float bhop_speed = 0.0f;
        float bhop_speed_decrease_rate = 8.0f;
        float bhop_speed_increase_rate = 13.0f;
        float jump_force = 12.2f;
#ifdef WSL
        float sensitivity = 0.5;
#else
        float sensitivity = 0.1;
#endif
        bool is_grounded = false;
        double delta_accumulator = 0.0f;
        glm::fvec3 forward = glm::fvec3(1.0f);

        std::bitset<8> last_collision_direction_flags = 0;
        glm::fvec3 controller_friction = glm::fvec3(10.0f, 0.0f, 10.0f);
        glm::fvec3 freecam_friction = glm::fvec3(8.0f);
        float gravity = 35.0f;
        TriggerBoxCollider main_collider_offset = {{-0.5f, -2.0f, -0.5f}, {0.5f, 0.5f, 0.5f}};
        TriggerBoxCollider ground_collider_offset = {{-0.2f, -2.2f, -0.2f}, {0.2f, -2.0f, 0.2f}};
        bool freecam_enabled = false;

        glm::dvec2 mouse_offset_since_last_update = glm::vec2(0.0f);
        glm::dvec2 last_mouse_pos = glm::vec2(0.0f);
        bool input_enabled = true;
        std::bitset<8> direction_flags = 0;


    private:
        std::function<void(const glm::mat4& view_matrix)> view_matrix_update_callback;
        Input* input;

        void OnKeyCallback(int key, int, int action, int);
        void OnCursorPosCallback(double xpos, double ypos);

        void UpdateFreecamController(BoxColliderConstView colliders, double delta_time);
        void UpdateCharacterController(BoxColliderConstView colliders, double delta_time);
    };
}
