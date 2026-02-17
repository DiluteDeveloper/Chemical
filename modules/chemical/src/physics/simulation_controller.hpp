#pragma once

#include "physics/physics_body.hpp"
#include "util/transform.hpp"

namespace Chemical {

  namespace Physics {

    class SimulationController {

      void PhysicsUpdate();

      PhysicsBody rt_body_a;
      PhysicsBody rt_body_b;

    public:
      SimulationController(Transform &body_a, Transform &body_b);

      void Update();
      void ResetSimulation();

      // PhysicsBody initial_body_a; todo
      // PhysicsBody initial_body_b; todo

      bool running = false;

      unsigned int speed_multiplier = 1;
    };
  } // namespace Physics
} // namespace Chemical
