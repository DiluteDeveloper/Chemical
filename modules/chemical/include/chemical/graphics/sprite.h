#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  class Sprite {
  public:
    glm::vec2 size = glm::vec2(1.0f);
    glm::vec3 tint = glm::vec3(255.0f);
    std::string transform_id = "default";
    std::string material_id = "default";
  };

  // class GLSprite {
  // protected:
  //   friend class Renderer;
  //
  //   static uint32_t GenerateVAO();
  // };

} // namespace Chemical
