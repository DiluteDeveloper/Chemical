#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  namespace Graphics { class Renderer; }

  using ObjectID = std::string;

  class Camera {
  public:
    Camera(const ObjectID &transform_id, float zoom_level = 0.0f);
    void SetZoomLevel(float zoom_level);
    float GetZoomLevel();

    ObjectID transform_id = "default";

  protected:
    friend class Graphics::Renderer;

    glm::mat3 projection_matrix = glm::mat3(1.0f);

    float zoom_level = 0.0f;
  };
} // namespace Chemical
