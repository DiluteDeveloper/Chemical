#include "simulation_controller.hpp"
#include "gui/physics.hpp"
#include "physics/kepler_orbit.hpp"

namespace Chemical {

  namespace Physics {
    SimulationController::SimulationController(Transform &body_a,
                                               Transform &body_b)
        : rt_body_a(body_a.position, 100, glm::dvec3(0, -0.28f, -0.18f)),
          rt_body_b(body_b.position, 10000) {}

    void SimulationController::Update() {
      GUI::RenderPhysicsMenu(*this);
      if (running) {
        PhysicsUpdate();
      }
    }
    void SimulationController::PhysicsUpdate() {
      for (unsigned int i = 0; i < speed_multiplier; i++) {
        Physics::ApplyKeplerOrbit(rt_body_a, rt_body_b);

        rt_body_a.position += rt_body_a.velocity;
        rt_body_b.position += rt_body_b.velocity;
      }
    }

  } // namespace Physics
} // namespace Chemical
