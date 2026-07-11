use crate::utility::RGBColour;

pub struct PointLight {
    pub position: cgmath::Point3<f32>,
    pub strength: f32,
    pub colour: RGBColour,
}
