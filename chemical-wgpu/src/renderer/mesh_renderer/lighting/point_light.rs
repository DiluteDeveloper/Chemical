#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct PointLight {
    pub position: [f32; 3],
    pub strength: f32,
    pub colour: [f32; 3],
    _pad1: f32,
}

impl PointLight {
    pub fn new(position: [f32; 3], colour: [f32; 3], strength: f32) -> Self {
        PointLight {
            position: position,
            strength: strength,
            colour: colour,
            _pad1: 0.0,
        }
    }
}
