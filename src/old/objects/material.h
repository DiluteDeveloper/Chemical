#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  struct Material {
    glm::uvec3 tint = glm::uvec3(255);
    std::string shader_id = "default";
    std::string texture_id = "default";
  };

} // namespace Chemical
