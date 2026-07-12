use crate::utility::RGBColour;

#[derive(Clone)]
pub struct DirectionalLight {
    pub orientation: cgmath::Quaternion<f32>,
    pub strength: f32,
    pub colour: RGBColour,
}
