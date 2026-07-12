use cgmath::SquareMatrix;
use chemical_engine::scene::types::{DirectionalLight, Transform};

#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct ShaderDirectionalLight {
    pub colour: [f32; 3],
    pub strength: f32,
    pub projection: [[f32; 4]; 4],
}

impl From<&DirectionalLight> for ShaderDirectionalLight {
    fn from(dl: &DirectionalLight) -> Self {
        // Temporarily exists for testing

        let mut t = Transform::default();
        t.position = (20.0, 20.0, 20.0).into();
        t.orientation = dl.orientation;
        let transform_mat: cgmath::Matrix4<f32> = (&t).into();
        let proj = cgmath::ortho(-20.0, 20.0, -20.0, 20.0, 0.1, 100.0);
        let m = proj * transform_mat.invert().unwrap();

        Self {
            strength: dl.strength,
            colour: (dl.colour.r, dl.colour.g, dl.colour.b).into(),
            projection: m.into(),
        }
    }
}
