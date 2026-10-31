#pragma once

#include "glm/ext/matrix_clip_space.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>

namespace Chemical {

  // Currently doesnt take screen size into account (should in future)
  struct Camera {
  public:
    Camera(float zoom_level = 0.0f) : zoom_level(zoom_level) {
      proj = glm::ortho(0.0f, (1280.0f + -(zoom_level * 1.28f)) / 100.0f, 0.0f,
                        (720.0f + -(zoom_level * 0.72f)) / 100.0f, 0.0f, 100.0f);
    }
    float GetZoomLevel() { return zoom_level; }
    void SetZoomLevel(float zoom_level) {
      this->zoom_level = zoom_level;
      proj = glm::ortho(0.0f, (1280.0f + -(zoom_level * 1.28f)) / 100.0f, 0.0f,
                        (720.0f + -(zoom_level * 0.72f)) / 100.0f, 0.0f, 100.0f);
    }

    std::string transform_id = "";

    // TODO: Make cached
    glm::mat3 GetProjectionMatrix() { return proj; }

  protected:
    glm::mat3 proj;
    float zoom_level = 0.0f;
  };
} // namespace Chemical
