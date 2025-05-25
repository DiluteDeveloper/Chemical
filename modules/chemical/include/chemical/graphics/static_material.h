#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Chemical {
  namespace Graphics {

    using MaterialID = std::string;
    struct Material {

      glm::vec3 albedo = glm::vec3(1.0f, 1.0f, 1.0f);

      Material(const glm::vec3 &albedo) : albedo(albedo) {}
    };
  } // namespace Graphics
} // namespace Chemical
