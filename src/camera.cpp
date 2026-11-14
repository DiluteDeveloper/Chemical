#include <spdlog/spdlog.h>
#include "camera.h"
#include "input.h"

namespace Chemical {

    Camera::Camera(float speed, float sensitivity) :
        speed(speed), sensitivity(sensitivity) {
        Input::SubscribeToCursorPosCallback(Camera::OnCursorPosCallback, this);
        Input::SubscribeToKeyCallback(Camera::OnKeyCallback, this);
    }
    Camera::~Camera() {
        Input::UnsubscribeFromCursorPosCallback(this);
        Input::UnsubscribeFromKeyCallback(this);
    }

    void Camera::Update(wgpu::Queue queue, wgpu::Buffer view_buffer) {


        if(direction_bitmask) {
            Move();
            queue.WriteBuffer(view_buffer, 0, &glm::inverse(transform.AsMat4())[0][0], sizeof(glm::mat4));
            mouse_moved_this_frame = false;
        } else if (mouse_moved_this_frame) {
            queue.WriteBuffer(view_buffer, 0, &glm::inverse(transform.AsMat4())[0][0], sizeof(glm::mat4));
            mouse_moved_this_frame = false;
        }



    }
    void Camera::OnKeyCallback(int key, int, int action, int, void* data) {
        Camera& camera = *reinterpret_cast<Camera*>(data);

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

        camera.direction_bitmask ^= (uint8_t)search->second;

    }

    void Camera::OnCursorPosCallback(double xpos, double ypos, void* data) {
        Camera& camera = *reinterpret_cast<Camera*>(data);

        camera.transform.rotation.y += static_cast<float>(xpos - camera.old_mouse_x) * camera.sensitivity;
        camera.transform.rotation.x += static_cast<float>(ypos - camera.old_mouse_y) * camera.sensitivity;

        camera.transform.rotation.x = glm::clamp(camera.transform.rotation.x, -85.0f, 85.0f);

        camera.old_mouse_x = xpos;
        camera.old_mouse_y = ypos;

        camera.mouse_moved_this_frame = true;

        // camera.callback(glm::inverse(camera.transform.AsMat4()));
    }

    void Camera::Move() {
        glm::fmat4 matrix = transform.AsMat4();
        glm::fvec3 right = matrix[0];
        glm::fvec3 up = glm::vec3(0.0, 1.0, 0.0);
        glm::fvec3 forward = matrix[2];
        glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

        if(direction_bitmask & (uint8_t)MoveDirection::Forwards)
            transform.position += front * speed;
        if(direction_bitmask & (uint8_t)MoveDirection::Backwards)
            transform.position -= front * speed;
        if(direction_bitmask & (uint8_t)MoveDirection::Right)
            transform.position += right * speed;
        if(direction_bitmask & (uint8_t)MoveDirection::Left)
            transform.position -= right * speed;
        if(direction_bitmask & (uint8_t)MoveDirection::Up)
            transform.position += up * speed;
        if(direction_bitmask & (uint8_t)MoveDirection::Down)
            transform.position -= up * speed;

    }
}
