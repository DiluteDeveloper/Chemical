use crate::{
    scene::types::{DirectionalLight, Transform},
    utility::RGBColour,
};
use glam::{
    Mat4, Vec3,
    camera::rh::{proj::directx, view::look_at_mat4},
};

#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct ShaderDirectionalLight {
    pub colour: RGBColour,
    pub strength: f32,
    pub dir: Vec3,
    pub _pad: f32,
    pub projection: Mat4,
}

impl From<&DirectionalLight> for ShaderDirectionalLight {
    fn from(dl: &DirectionalLight) -> Self {
        // Temporarily exists for testing

        //let mut t = Transform::default();
        //t.position = (20.0, 20.0, 20.0).into();
        //t.orientation = dl.orientation;
        let proj = directx::orthographic(-20.0, 20.0, -20.0, 20.0, 0.1, 100.0);
        let dir = dl.orientation.to_axis_angle().0;
        let m = proj * look_at_mat4(dir * 20.0, (0.0, 0.0, 0.0).into(), Vec3::Y);

        Self {
            strength: dl.strength,
            colour: dl.colour,
            dir,
            _pad: 0.0,
            projection: m,
        }
    }
}
