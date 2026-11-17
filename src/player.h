#pragma once

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <functional>
#include "utils/transform.h"
#include "physics/physics.h"

namespace Chemical {

    class Player {
    public:
        typedef std::function<void(glm::mat4)> OnViewMatrixUpdated;

        Player();
        // Camera(const Camera&) = delete;
        // Camera(Camera&&) = delete;
        // Camera operator=(const Camera&) = delete;
        // Camera operator=(Camera&&) = delete;

        ~Player();
        void Update(wgpu::Queue queue, wgpu::Buffer view_buffer, double _delta_time);

        float speed = 0.31;
        float vertical_speed = 0.05;
        float jump_force = 0.05;
#ifdef WSL
        float sensitivity = 0.5;
#else
        float sensitivity = 0.2;
#endif

        bool freecam = false;
    private:
        double delta_time = 0;

        std::vector<Extent3D> other_colliders;

        enum class MoveDirection:uint8_t {
            None = 0b00000000, 
            Up = 0b00000001, 
            Down = 0b00000010, 
            Left = 0b00000100, 
            Right = 0b00001000, 
            Forwards = 0b00010000, 
            Backwards = 0b00100000
        };
        PhysicsBody body;

        Transform transform;
        glm::mat4 current_matrix;

        uint8_t direction_bitmask = 0;

        double old_mouse_x = 0.0;
        double old_mouse_y = 0.0;

        static void OnKeyCallback(int key, int, int action, int, void* data);
        static void OnCursorPosCallback(double xpos, double ypos, void* data);

        void Move();
    };
}
