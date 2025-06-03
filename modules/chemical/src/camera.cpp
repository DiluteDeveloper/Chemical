#include "chemical/camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

  Camera::Camera(const ObjectID &transform_id, float zoom_level)
      : transform_id(transform_id), zoom_level(zoom_level) {

    projection_matrix = glm::ortho(0.0f, (1280.0f + -(zoom_level * 1.28f)) / 100.0f, 0.0f,
                                   (720.0f + -(zoom_level * 0.72f)) / 100.0f, 0.0f, 100.0f);
  }
  void Camera::SetZoomLevel(float zoom_level) {
    this->zoom_level = zoom_level;
    projection_matrix = glm::ortho(0.0f, (1280.0f + -(zoom_level * 1.28f)) / 100.0f, 0.0f,
                                   (720.0f + -(zoom_level * 0.72f)) / 100.0f, 0.0f, 100.0f);
  }
  float Camera::GetZoomLevel() {
    return zoom_level;
  }
} // namespace Chemical
