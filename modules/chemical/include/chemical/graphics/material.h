#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {

  using ObjectID = std::string;
  namespace Graphics {

    struct Material {

      glm::vec3 tint = glm::vec3(255.0f);
      ObjectID texture_id = "default";

      Material(const ObjectID &texture_id = "default", const glm::vec3 &tint = glm::vec3(255.0f))
          : tint(tint), texture_id(texture_id) {}
    };
  } // namespace Graphics
} // namespace Chemical
