#pragma once

#include "glm/geometric.hpp"
#include "physics/physics_body.hpp"
#include "physics/simulation_controller.hpp"

namespace Chemical {

  namespace Physics {

    // Gravity constant
    constexpr double GRAVITY = 9.81f;

    // Force from B gravitating towards A
    inline void ApplyKeplerOrbit(SimulationDataLogger &logger,
                                 const glm::vec3 &position_a,
                                 const glm::vec3 &position_b, PhysicsBody &a,
                                 PhysicsBody &b) {

      // relative position vector A <- B
      glm::dvec3 rel = position_a - position_b;

      // Unit vector A <- B
      glm::dvec3 unit_vector = glm::normalize(rel);

      // Length of relative position vector A <- B
      double length = std::sqrt(std::pow(rel.x, 2) + std::pow(rel.y, 2) +
                                std::pow(rel.z, 2));

      // Force to apply to A
      // glm::dvec3 force_a =
      //     ((-GRAVITY * a.mass * b.mass) / std::pow(length, 2)) * unit_vector;

      // the mass of an object can be removed from the force equation as ma=f,
      // as in, the product of mass and acceleration is force, mass is on the
      // left side (m) and its in the formula for force

      glm::dvec3 acceleration_a =
          ((-GRAVITY * b.mass) / std::pow(length, 2)) * unit_vector;

      // Force to apply to B
      glm::dvec3 acceleration_b =
          ((GRAVITY * a.mass) / std::pow(length, 2)) * unit_vector;

      // glm::dvec3 acceleration_a = force_a / glm::dvec3(a.mass);
      //
      // glm::dvec3 acceleration_b = force_b / glm::dvec3(b.mass);
      a.velocity += acceleration_a;

      b.velocity += acceleration_b;

      logger.LogDistance(std::abs(length));
      logger.LogAccelerationA(acceleration_a);
      logger.LogAccelerationB(acceleration_b);
      logger.LogVelocity(a.velocity);
    }
  } // namespace Physics
} // namespace Chemical
