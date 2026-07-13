#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct RGBColour {
    pub r: f32,
    pub g: f32,
    pub b: f32,
}

impl From<&RGBColour> for (f32, f32, f32) {
    fn from(col: &RGBColour) -> Self {
        (col.r, col.g, col.b)
    }
}
impl From<(f32, f32, f32)> for RGBColour {
    fn from(v: (f32, f32, f32)) -> Self {
        Self {
            r: v.0,
            g: v.1,
            b: v.2,
        }
    }
}
