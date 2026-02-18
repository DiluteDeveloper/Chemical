#pragma once

#include "glm/geometric.hpp"
#include "physics/physics_body.hpp"

namespace Chemical {

  namespace Physics {

    // Gravity constant
    constexpr double GRAVITY = 9.81f;

    // Force from B gravitating towards A
    inline void ApplyKeplerOrbit(const glm::vec3 &position_a,
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
      glm::dvec3 force_a =
          ((-GRAVITY * a.mass * b.mass) / std::pow(length, 2)) * unit_vector;

      // Force to apply to B
      glm::dvec3 force_b =
          ((GRAVITY * a.mass * b.mass) / std::pow(length, 2)) * unit_vector;

      glm::dvec3 acceleration_a = force_a / glm::dvec3(a.mass);

      glm::dvec3 acceleration_b = force_b / glm::dvec3(b.mass);

      // SPDLOG_INFO("acceleration: {} {} {} | {} {} {}", acceleration_a.x,
      //             acceleration_a.y, acceleration_a.z, acceleration_b.x,
      //             acceleration_b.y, acceleration_b.z);

      // glm::dvec3 velocity_a = acceleration_a - acceleration_b;
      // glm::dvec3 velocity_b = acceleration_b - acceleration_a;

      // SPDLOG_INFO("velocity: {} {} {} | {} {} {}", velocity_a.x,
      // velocity_a.y,
      //             velocity_a.z, velocity_b.x, velocity_b.y, velocity_b.z);

      a.velocity += acceleration_a;

      b.velocity += acceleration_b;
    }
  } // namespace Physics
} // namespace Chemical
