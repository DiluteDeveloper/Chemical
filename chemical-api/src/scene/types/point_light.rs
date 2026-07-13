use crate::utility::RGBColour;

pub struct PointLight {
    pub position: glam::Vec3,
    pub strength: f32,
    pub colour: RGBColour,
}
