use chemical_engine::scene::types::PointLight;

#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct ShaderPointLight {
    pub position: [f32; 3],
    pub strength: f32,
    pub colour: [f32; 3],
    _pad: i32,
}

impl From<&PointLight> for ShaderPointLight {
    fn from(pl: &PointLight) -> Self {
        ShaderPointLight {
            position: pl.position.into(),
            strength: pl.strength,
            colour: (pl.colour.r, pl.colour.g, pl.colour.b).into(),
            _pad: 0,
        }
    }
}
