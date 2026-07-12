use crate::utility::RGBColour;

#[derive(Clone)]
pub struct DirectionalLight {
    pub orientation: glam::Quat,
    pub strength: f32,
    pub colour: RGBColour,
}
