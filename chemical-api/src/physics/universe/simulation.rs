use crate::physics::universe::celestial_body::CelestialBody;
use crate::physics::universe::kepler_orbit;

pub struct UniverseSimulation {
    pub celestial_bodies: [CelestialBody; 9],

    tick_count: u64,
}

impl UniverseSimulation {
    pub fn new() -> Self {
        Self {
            celestial_bodies: [
                /*CelestialBody {
                    position: (-6550.0, 100.0, 0.0).into(),
                    mass: 1000.0,
                    velocity: (0.0, -0.1, -5.58).into(),
                },
                CelestialBody {
                    position: (0.0, 0.1, 0.0).into(),
                    mass: 1500000.0,
                    velocity: (0.1, 0.0, 0.0).into(),
                },
                CelestialBody {
                    position: (-10550.0, 300.0, 0.0).into(),
                    mass: 7000.0,
                    velocity: (0.0, -1.0, -7.59).into(),
                },
                CelestialBody {
                    position: (10550.0, -50.0, 0.0).into(),
                    mass: 3000.0,
                    velocity: (0.0, 3.0, 7.58).into(),
                },
                CelestialBody {
                    position: (8550.0, -500.0, 0.0).into(),
                    mass: 1000.0,
                    velocity: (0.0, 1.0, 7.58).into(),
                },*/
                CelestialBody {
                    // sun
                    position: (0.0, 0.0, 0.0).into(),
                    mass: 19890.0,
                    velocity: (0.0, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // earth
                    position: (0.0, 0.0, 14960.0).into(),
                    mass: 5.97,
                    velocity: (0.298, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // mars
                    position: (0.0, 0.0, 22800.0).into(),
                    mass: 0.642,
                    velocity: (0.241, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // venus
                    position: (0.0, 0.0, 10820.0).into(),
                    mass: 4.87,
                    velocity: (0.35, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // jupiter
                    position: (0.0, 0.0, 77850.0).into(),
                    mass: 1898.0,
                    velocity: (0.131, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // saturn
                    position: (0.0, 0.0, 143200.0).into(),
                    mass: 568.0,
                    velocity: (0.097, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // uranus
                    position: (0.0, 0.0, 286700.0).into(),
                    mass: 86.8,
                    velocity: (0.068, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // neptune
                    position: (0.0, 0.0, 451500.0).into(),
                    mass: 102.0,
                    velocity: (0.054, 0.0, 0.0).into(),
                },
                CelestialBody {
                    // pluto
                    position: (0.0, 0.0, 590640.0).into(),
                    mass: 0.013,
                    velocity: (0.047, 0.0, 0.0).into(),
                },
            ],
            tick_count: 0,
        }
    }

    pub fn tick(&mut self, delta: f64) {
        for i in 0..self.celestial_bodies.len() {
            let (left, right) = self.celestial_bodies.split_at_mut(i + 1);
            let body_a = &mut left[i];
            for body_b in right.iter_mut() {
                let result = kepler_orbit::simulate_kepler_orbit(body_a, body_b);
                body_a.velocity += result.acceleration_a * delta as f32;
                body_b.velocity += result.acceleration_b * delta as f32;
            }
            body_a.tick();
        }
        self.tick_count += 1;
    }

    pub fn get_tick_count(&self) -> u64 {
        self.tick_count
    }
}
