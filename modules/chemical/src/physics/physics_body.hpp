#pragma once

#include <glm/glm.hpp>

namespace Chemical {
  namespace Physics {

    struct PhysicsBody {

      // Intended to be m/s
      glm::dvec3 velocity = glm::dvec3(0);

      // Intended to be kg
      float mass = 1.0f;

      PhysicsBody(float mass = 1.0f, glm::dvec3 velocity = glm::dvec3(0))
          : mass(mass), velocity(velocity) {}
    };
  } // namespace Physics
} // namespace Chemical
