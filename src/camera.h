#pragma once

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <functional>
#include "utils/transform.h"

namespace Chemical {

    class Camera {
    public:
        typedef std::function<void(glm::mat4)> OnViewMatrixUpdated;

        Camera();
        // Camera(const Camera&) = delete;
        // Camera(Camera&&) = delete;
        // Camera operator=(const Camera&) = delete;
        // Camera operator=(Camera&&) = delete;

        ~Camera();
        void Update(wgpu::Queue queue, wgpu::Buffer view_buffer);

        float speed = 0.01;
        float sensitivity = 0.1;
    private:

        enum class MoveDirection:uint8_t {
            None = 0b00000000, 
            Up = 0b00000001, 
            Down = 0b00000010, 
            Left = 0b00000100, 
            Right = 0b00001000, 
            Forwards = 0b00010000, 
            Backwards = 0b00100000
        };
        Utils::Transform transform;
        uint8_t direction_bitmask = 0;

        double old_mouse_x = 0.0;
        double old_mouse_y = 0.0;
        bool mouse_moved_this_frame = false;

        static void OnKeyCallback(int key, int, int action, int, void* data);
        static void OnCursorPosCallback(double xpos, double ypos, void* data);

        void Move();
    };
}
