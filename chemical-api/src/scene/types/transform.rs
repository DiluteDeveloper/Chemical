use glam::{Mat4, Quat, Vec3};
#[derive(Copy, Clone, Debug)]
pub struct Transform {
    pub position: Vec3,
    pub scale: Vec3,
    pub orientation: Quat,
}
impl Transform {
    pub fn to_matrix(&self) -> Mat4 {
        Mat4::from_scale_rotation_translation(self.scale, self.orientation, self.position)
    }
}

impl Default for Transform {
    fn default() -> Transform {
        Transform {
            position: (0.0, 0.0, 0.0).into(),
            scale: (1.0, 1.0, 1.0).into(),
            orientation: Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
        }
    }
}
/*
impl From<&Transform> for Mat4 {
    fn from(v: &Transform) -> Self {
        Mat4::from_scale_rotation_translation(v.scale, v.orientation, v.position)
    }
}*/
/*impl From<&Transform> for [[f32; 4]; 4] {
    fn from(v: &Transform) -> Self {
        Mat4::from_scale_rotation_translation(v.scale, v.orientation, v.position).to_cols_array_2d()
    }
}
*/
