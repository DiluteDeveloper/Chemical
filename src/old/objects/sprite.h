#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  class Sprite {
  public:
    glm::vec2 size = glm::vec2(1.0f);
    std::string transform_id = "default";
    std::string material_id = "default";
  };

} // namespace Chemical
