#pragma once

#include "texture.h"

#include <glm/glm.hpp>
#include <string>

namespace Chemical {
  namespace Graphics {

    using MaterialID = std::string;
    struct Material {

      glm::vec3 albedo = glm::vec3(255.0f, 255.0f, 255.0f);
      TextureID texture_id = "default";
    };
  } // namespace Graphics
} // namespace Chemical
