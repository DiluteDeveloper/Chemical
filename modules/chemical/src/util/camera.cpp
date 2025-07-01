#include "camera.hpp"
#include "glfw/glfw3.h"

namespace Chemical {

  CameraController::CameraController(GLFWwindow *window, float speed,
                                     float sensitivity)
      : speed(speed), sensitivity(sensitivity) {
    glfwGetCursorPos(window, &old_x, &old_x);
  }
  void CameraController::Update(GLFWwindow *window) {
    glm::fmat4 matrix = transform.ToMatrix();
    glm::fvec3 right = matrix[0];
    glm::fvec3 up = matrix[1];
    glm::fvec3 forward = matrix[2];
    glm::fvec3 front = glm::normalize(glm::fvec3(forward.x, 0, forward.z));

    if (glfwGetKey(window, GLFW_KEY_W)) {
      transform.position -= front * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
      transform.position += front * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
      transform.position -= right * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
      transform.position += right * speed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
      transform.position.y += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
      transform.position.y -= speed;
    }

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    transform.rotation.y -= static_cast<float>(x - old_x) * sensitivity;
    transform.rotation.x -= static_cast<float>(y - old_y) * sensitivity;

    transform.rotation.x = glm::clamp(transform.rotation.x, -85.0f, 85.0f);

    old_x = x;
    old_y = y;
  }
} // namespace Chemical
