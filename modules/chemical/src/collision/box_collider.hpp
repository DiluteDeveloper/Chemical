#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "rendering/mesh.hpp"
#include "util/transform.hpp"

namespace Chemical {

  struct BoxCollider3D {
    // How far each axis goes in each direction from the center of the box
    glm::vec3 extents = glm::vec3(1.0f);

    glm::vec3 position;

    BoxCollider3D(float extents, const glm::vec3 &position = glm::vec3(1.0f));
    BoxCollider3D(const glm::vec3 &extents,
                  const glm::vec3 &position = glm::vec3(1.0f));

    Mesh AsMesh();

    bool IsCollidingWith(const BoxCollider3D &other) const;
  };
} // namespace Chemical
