#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct PointLight {
    pub position: [f32; 3],
    _pad0: f32,
    pub diffuse: [f32; 3],
    _pad1: f32,
}

impl PointLight {
    pub fn new(position: [f32; 3], diffuse: [f32; 3]) -> Self {
        PointLight {
            position: position,
            _pad0: 0.0,
            diffuse: diffuse,
            _pad1: 0.0,
        }
    }
}
