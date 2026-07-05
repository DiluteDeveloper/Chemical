pub mod celestial_body;
mod kepler_orbit;

use crate::physics::universe_simulation::celestial_body::CelestialBody;

pub struct UniverseSimulation {
    pub body_a: CelestialBody,
    pub body_b: CelestialBody,
    pub body_c: CelestialBody,
}

impl UniverseSimulation {
    pub fn new() -> Self {
        Self {
            body_a: CelestialBody {
                position: (2550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, 0.0, -5.58).into(),
            },
            body_b: CelestialBody {
                position: (0.0, 0.0, 0.0).into(),
                mass: 10000.0,
                velocity: (1.0, 1.0, 0.0).into(),
            },
            body_c: CelestialBody {
                position: (-2550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, 0.0, 5.58).into(),
            },
        }
    }

    pub fn tick(&mut self) {
        let kepler_orbit_result_a_b =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_b);
        let kepler_orbit_result_b_c =
            kepler_orbit::simulate_kepler_orbit(&self.body_b, &self.body_c);
        let kepler_orbit_result_a_c =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_c);

        self.body_a.velocity += kepler_orbit_result_a_b.acceleration_a;
        self.body_a.velocity += kepler_orbit_result_a_c.acceleration_a;
        self.body_b.velocity += kepler_orbit_result_a_b.acceleration_b;
        self.body_b.velocity += kepler_orbit_result_b_c.acceleration_a;
        self.body_c.velocity += kepler_orbit_result_b_c.acceleration_b;
        self.body_c.velocity += kepler_orbit_result_a_c.acceleration_b;

        self.body_a.position += self.body_a.velocity;
        self.body_b.position += self.body_b.velocity;
        self.body_c.position += self.body_c.velocity;
    }
}
