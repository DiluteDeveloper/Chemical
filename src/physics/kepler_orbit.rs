use crate::physics::body::Body as PhysicsBody;
use cgmath::{InnerSpace, Vector3};

const GRAVITY: f32 = 9.81;

pub(super) struct KeplerOrbitResult {
    pub(super) acceleration_a: Vector3<f32>,
    pub(super) acceleration_b: Vector3<f32>,
}

pub fn simulate_kepler_orbit(body_a: &PhysicsBody, body_b: &PhysicsBody) -> KeplerOrbitResult {
    let rel = body_a.transform.position - body_b.transform.position;

    let unit_vector = rel.normalize();

    let length = (rel.x.powf(2.0) + rel.y.powf(2.0) + rel.z.powf(2.0)).sqrt();

    let acceleration_a = ((-GRAVITY * body_b.mass) / length.powf(2.0)) * unit_vector;
    let acceleration_b = ((GRAVITY * body_a.mass) / length.powf(2.0)) * unit_vector;

    KeplerOrbitResult {
        acceleration_a,
        acceleration_b,
    }
}

