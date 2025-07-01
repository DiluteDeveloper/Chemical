#pragma once

#include "glfw/glfw3.h"

#include "transform.hpp"

namespace Chemical {

  // TODO: Create input class so window doesnt have to be passed to camera
  // controller
  class CameraController {
  public:
    Transform transform;

    CameraController(GLFWwindow *window, float speed = 0.1f,
                     float sensitivity = 0.08f);
    void Update(GLFWwindow *window);

    float speed = 0.1f;
    float sensitivity = 0.08f;

  protected:
    double old_x, old_y;
  };
} // namespace Chemical
