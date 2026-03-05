use crate::utility::transform::Transform;
use cgmath::Vector3;

pub(super) struct Body {
    pub(super) transform: Transform,
    pub(super) mass: f32,
    pub(super) velocity: Vector3<f32>,
}

impl Body {
    pub(super) fn new(transform: &Transform, mass: f32, velocity: (f32, f32, f32)) -> Self {
        Self {
            transform: transform.clone(),
            mass: mass,
            velocity: velocity.into(),
        }
    }
}
