pub mod celestial_body;
mod kepler_orbit;

use crate::physics::universe_simulation::celestial_body::CelestialBody;

pub struct UniverseSimulation {
    pub body_a: CelestialBody,
    pub body_b: CelestialBody,
}

impl UniverseSimulation {
    pub fn new() -> Self {
        Self {
            body_a: CelestialBody {
                position: (2550.0, 0.0, 0.0).into(),
                mass: 100.0,
                velocity: (0.0, 0.0, -5.58).into(),
            },
            body_b: CelestialBody {
                position: (0.0, 0.0, 0.0).into(),
                mass: 10000.0,
                velocity: (0.0, 0.0, 0.0).into(),
            },
        }
    }

    pub fn tick(&mut self) {
        let kepler_orbit_result = kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_b);

        self.body_a.velocity += kepler_orbit_result.acceleration_a;
        self.body_b.velocity += kepler_orbit_result.acceleration_b;
        self.body_a.position += self.body_a.velocity;
        self.body_b.position += self.body_b.velocity;
    }
}
