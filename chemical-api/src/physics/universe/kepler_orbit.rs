use crate::physics::universe::celestial_body::CelestialBody;

const GRAVITY: f32 = 9.81;

pub struct KeplerOrbitResult {
    pub acceleration_a: glam::Vec3,
    pub acceleration_b: glam::Vec3,
}

pub fn simulate_kepler_orbit(body_a: &CelestialBody, body_b: &CelestialBody) -> KeplerOrbitResult {
    let rel = body_a.position - body_b.position;

    let unit_vector = rel.normalize();

    let length = (rel.x.powf(2.0) + rel.y.powf(2.0) + rel.z.powf(2.0)).sqrt();

    let acceleration_a = ((-GRAVITY * body_b.mass) / length.powf(2.0)) * unit_vector;
    let acceleration_b = ((GRAVITY * body_a.mass) / length.powf(2.0)) * unit_vector;

    KeplerOrbitResult {
        acceleration_a,
        acceleration_b,
    }
}
