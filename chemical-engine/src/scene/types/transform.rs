use cgmath::{EuclideanSpace, Matrix4, Point3, Quaternion};

#[derive(Copy, Clone, Debug)]
pub struct Transform {
    pub position: Point3<f32>,
    pub scale: Point3<f32>,
    pub orientation: Quaternion<f32>,
}

impl Default for Transform {
    fn default() -> Transform {
        Transform {
            position: (0.0, 0.0, 0.0).into(),
            scale: (1.0, 1.0, 1.0).into(),
            orientation: (0.0, 0.0, 0.0, -1.0).into(),
        }
    }
}

impl From<&Transform> for Matrix4<f32> {
    fn from(v: &Transform) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let rotation_matrix = Matrix4::from(v.orientation);
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.x, v.scale.y, v.scale.z);

        translation_matrix * rotation_matrix * scale_matrix
    }
}
impl From<&Transform> for [[f32; 4]; 4] {
    fn from(v: &Transform) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let rotation_matrix = Matrix4::from(v.orientation);
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.x, v.scale.y, v.scale.z);

        (translation_matrix * rotation_matrix * scale_matrix).into()
    }
}
