#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  using ObjectID = std::string;
  namespace Graphics {

    struct Material {

      glm::vec3 albedo = glm::vec3(255.0f, 255.0f, 255.0f);
      ObjectID texture_id = "default";
    };
  } // namespace Graphics
} // namespace Chemical
