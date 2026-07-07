use crate::physics::universe::celestial_body::CelestialBody;
use crate::physics::universe::kepler_orbit;

pub struct UniverseSimulation {
    pub body_a: CelestialBody,
    pub body_b: CelestialBody,
    pub body_c: CelestialBody,
    pub body_d: CelestialBody,
    pub body_e: CelestialBody,

    tick_count: u64,
}

impl UniverseSimulation {
    pub fn new() -> Self {
        Self {
            body_a: CelestialBody {
                position: (-8550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, 0.0, -5.58).into(),
                trail: Vec::new(),
            },
            body_b: CelestialBody {
                position: (0.0, 0.1, 0.0).into(),
                mass: 100000.0,
                velocity: (0.1, 0.0, 0.0).into(),
                trail: Vec::new(),
            },
            body_c: CelestialBody {
                position: (-10550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, 1.0, -7.59).into(),
                trail: Vec::new(),
            },
            body_d: CelestialBody {
                position: (10550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, 0.0, 7.58).into(),
                trail: Vec::new(),
            },
            body_e: CelestialBody {
                position: (8550.0, 0.0, 0.0).into(),
                mass: 1000.0,
                velocity: (0.0, -1.0, 7.58).into(),
                trail: Vec::new(),
            },
            tick_count: 0,
        }
    }

    pub fn tick(&mut self) {
        let kepler_orbit_result_a_b =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_b);
        let kepler_orbit_result_a_c =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_c);
        let kepler_orbit_result_a_d =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_d);
        let kepler_orbit_result_a_e =
            kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_e);

        let kepler_orbit_result_b_c =
            kepler_orbit::simulate_kepler_orbit(&self.body_b, &self.body_c);
        let kepler_orbit_result_b_d =
            kepler_orbit::simulate_kepler_orbit(&self.body_b, &self.body_d);
        let kepler_orbit_result_b_e =
            kepler_orbit::simulate_kepler_orbit(&self.body_b, &self.body_e);

        let kepler_orbit_result_c_d =
            kepler_orbit::simulate_kepler_orbit(&self.body_c, &self.body_d);
        let kepler_orbit_result_c_e =
            kepler_orbit::simulate_kepler_orbit(&self.body_c, &self.body_e);

        let kepler_orbit_result_d_e =
            kepler_orbit::simulate_kepler_orbit(&self.body_d, &self.body_e);

        self.body_a.velocity += kepler_orbit_result_a_b.acceleration_a;
        self.body_a.velocity += kepler_orbit_result_a_c.acceleration_a;
        self.body_a.velocity += kepler_orbit_result_a_d.acceleration_a;
        self.body_a.velocity += kepler_orbit_result_a_e.acceleration_a;

        self.body_b.velocity += kepler_orbit_result_a_b.acceleration_b;
        self.body_b.velocity += kepler_orbit_result_b_c.acceleration_a;
        self.body_b.velocity += kepler_orbit_result_b_d.acceleration_a;
        self.body_b.velocity += kepler_orbit_result_b_e.acceleration_a;

        self.body_c.velocity += kepler_orbit_result_a_c.acceleration_b;
        self.body_c.velocity += kepler_orbit_result_b_c.acceleration_b;
        self.body_c.velocity += kepler_orbit_result_c_d.acceleration_a;
        self.body_c.velocity += kepler_orbit_result_c_e.acceleration_a;

        self.body_d.velocity += kepler_orbit_result_a_d.acceleration_b;
        self.body_d.velocity += kepler_orbit_result_b_d.acceleration_b;
        self.body_d.velocity += kepler_orbit_result_c_d.acceleration_b;
        self.body_d.velocity += kepler_orbit_result_d_e.acceleration_a;

        self.body_e.velocity += kepler_orbit_result_a_e.acceleration_b;
        self.body_e.velocity += kepler_orbit_result_b_e.acceleration_b;
        self.body_e.velocity += kepler_orbit_result_c_e.acceleration_b;
        self.body_e.velocity += kepler_orbit_result_d_e.acceleration_b;

        self.body_a.tick(self.tick_count);
        self.body_b.tick(self.tick_count);
        self.body_c.tick(self.tick_count);
        self.body_d.tick(self.tick_count);
        self.body_e.tick(self.tick_count);

        self.tick_count += 1;
    }
}
