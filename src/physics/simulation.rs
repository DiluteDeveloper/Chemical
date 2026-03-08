use crate::physics::body::Body as PhysicsBody;
use crate::physics::kepler_orbit;
use crate::utility::transform::Transform;
use cgmath::Matrix4;

pub struct Simulation {
    body_a: PhysicsBody,
    body_b: PhysicsBody,
}

impl Simulation {
    pub fn new() -> Self {
        Self {
            body_a: PhysicsBody {
                transform: Transform {
                    position: (2550.0, 0.0, 0.0).into(),
                    scale: (20.0, 20.0, 20.0).into(),
                    orientation: (0.0, 0.0, 0.0, -1.0).into(),
                },
                mass: 100.0,
                velocity: (0.0, 0.0, -5.58).into(),
            },
            body_b: PhysicsBody {
                transform: Transform {
                    position: (0.0, 0.0, 0.0).into(),
                    scale: (200.0, 200.0, 200.0).into(),
                    orientation: (0.0, 0.0, 0.0, -1.0).into(),
                },
                mass: 10000.0,
                velocity: (0.0, 0.0, 0.0).into(),
            },
        }
    }

    pub fn update(&mut self) {
        let kepler_orbit_result = kepler_orbit::simulate_kepler_orbit(&self.body_a, &self.body_b);

        self.body_a.velocity += kepler_orbit_result.acceleration_a;
        self.body_b.velocity += kepler_orbit_result.acceleration_b;
        self.body_a.transform.position += self.body_a.velocity;
        self.body_b.transform.position += self.body_b.velocity;
    }

    pub fn get_body_a_transformation_matrix(&self) -> Matrix4<f32> {
        (&self.body_a.transform).into()
    }
    pub fn get_body_b_transformation_matrix(&self) -> Matrix4<f32> {
        (&self.body_b.transform).into()
    }
}
