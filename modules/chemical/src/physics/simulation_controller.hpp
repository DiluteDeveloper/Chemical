#pragma once

#include "physics/physics_body.hpp"
#include "rendering/orbit_line_mesh.hpp"
#include "util/transform.hpp"
#include <vector>

namespace Chemical {

  namespace Physics {

    class SimulationDataLogger {

      // Distance and total force
      std::vector<double> distance_data;
      std::vector<double> acceleration_a_data;
      std::vector<double> acceleration_b_data;
      std::vector<unsigned int> tick_index_data;
      std::vector<glm::vec3> velocity_data;

    public:
      bool is_logging = false;
      void LogDistance(double distance);
      void LogAccelerationA(const glm::vec3 &a);
      void LogAccelerationB(const glm::vec3 &b);
      void LogTickIndex(unsigned int idx);
      void LogVelocity(const glm::vec3 &velocity);
      void Export();
    };

    class SimulationController {
      void PhysicsUpdate();
      bool running = false;
      bool paused = false;

      unsigned int tick_idx = 0;

      Transform rt_transform_a;
      Transform rt_transform_b;

      Transform initial_transform_a;
      Transform initial_transform_b;

      SimulationDataLogger logger;

    public:
      std::vector<glm::vec3> positions_a;
      std::vector<glm::vec3> positions_b;

      OrbitLineMesh orbit_line_a;
      OrbitLineMesh orbit_line_b;

      SimulationController();

      void Update();
      void ResetSimulation();
      void Play();
      void Reset();
      void Pause();
      void ToggleLogging();
      bool IsLogging();

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
