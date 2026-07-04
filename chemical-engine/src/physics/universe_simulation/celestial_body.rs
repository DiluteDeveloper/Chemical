use cgmath::Point3;
use cgmath::Vector3;

pub struct CelestialBody {
    pub velocity: Vector3<f32>,
    pub position: Point3<f32>,
    pub mass: f32,
}
