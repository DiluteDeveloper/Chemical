#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "rendering/mesh.hpp"
#include "util/transform.hpp"

namespace Chemical {

  struct BoxCollider3D {
    // low_x  | low_y  | low_z
    // high_x | high_y | high_z
    glm::mat3x2 extents = glm::mat3x2(1.0f);

    glm::vec3 position;

    BoxCollider3D(float scale, const glm::vec3 &position = glm::vec3(1.0f));
    BoxCollider3D(const glm::vec3 &scale,
                  const glm::vec3 &position = glm::vec3(1.0f));

    Mesh AsMesh();

    bool IsCollidingWith(const BoxCollider3D &other) const;
  };
} // namespace Chemical
