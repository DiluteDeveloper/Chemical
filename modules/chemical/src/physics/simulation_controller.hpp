#pragma once

#include "physics/physics_body.hpp"
#include "util/transform.hpp"
#include <chrono>

namespace Chemical {

  namespace Physics {

    class SimulationController {

      void PhysicsUpdate();
      bool running = false;

      Transform rt_transform_a;
      Transform rt_transform_b;

      Transform initial_transform_a;
      Transform initial_transform_b;

    public:
      SimulationController();

      void Update();
      void ResetSimulation();
      void Play();
      void Reset();
      void Pause();

      Transform &GetActiveTransformA();
      Transform &GetActiveTransformB();

      float GetActiveMassA();
      float GetActiveMassB();
      void SetActiveMassA(float mass);
      void SetActiveMassB(float mass);

      glm::vec3 GetActiveVelocityA();
      glm::vec3 GetActiveVelocityB();
      void SetActiveVelocityA(const glm::vec3 &velocity);
      void SetActiveVelocityB(const glm::vec3 &velocity);

      bool IsRunning() { return running; }

      PhysicsBody rt_body_a;
      PhysicsBody rt_body_b;

      PhysicsBody initial_body_a;
      PhysicsBody initial_body_b;

      unsigned int speed_multiplier = 1;
    };
  } // namespace Physics
} // namespace Chemical
