#pragma once

#include <glm/glm.hpp>

namespace Chemical {
  namespace Physics {

    struct PhysicsBody {

      glm::vec3 &position;
      // Intended to be m/s
      glm::dvec3 velocity = glm::dvec3(0);

      // Intended to be kg
      float mass = 1.0f;

      PhysicsBody(glm::vec3 &position, float mass = 1.0f,
                  glm::dvec3 velocity = glm::dvec3(0))
          : position(position), mass(mass), velocity(velocity) {}
    };
  } // namespace Physics
} // namespace Chemical
