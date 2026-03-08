use crate::utility::transform::Transform;
use cgmath::Vector3;

pub(super) struct Body {
    pub(super) transform: Transform,
    pub(super) mass: f32,
    pub(super) velocity: Vector3<f32>,
}
