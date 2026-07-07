use cgmath::{EuclideanSpace, Matrix4, Point3, Quaternion};

#[derive(Copy, Clone, Debug)]
pub struct Transform {
    pub position: Point3<f32>,
    pub scale: (f32, f32, f32),
    pub orientation: Quaternion<f32>,
}

impl Default for Transform {
    fn default() -> Self {
        Self {
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
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.0, v.scale.1, v.scale.2);

        translation_matrix * rotation_matrix * scale_matrix
    }
}
impl From<&Transform> for [[f32; 4]; 4] {
    fn from(v: &Transform) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let rotation_matrix = Matrix4::from(v.orientation);
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.0, v.scale.1, v.scale.2);

        (translation_matrix * rotation_matrix * scale_matrix).into()
    }
}

#[derive(Copy, Clone, Debug)]
pub struct TransformNoScale {
    pub position: Point3<f32>,
    pub orientation: Quaternion<f32>,
}

impl Default for TransformNoScale {
    fn default() -> Self {
        Self {
            position: (0.0, 0.0, 0.0).into(),
            orientation: (0.0, 0.0, 0.0, -1.0).into(),
        }
    }
}
impl From<&TransformNoScale> for Matrix4<f32> {
    fn from(v: &TransformNoScale) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let rotation_matrix = Matrix4::from(v.orientation);

        translation_matrix * rotation_matrix
    }
}
impl From<&TransformNoScale> for [[f32; 4]; 4] {
    fn from(v: &TransformNoScale) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let rotation_matrix = Matrix4::from(v.orientation);

        (translation_matrix * rotation_matrix).into()
    }
}

#[derive(Copy, Clone, Debug)]
pub struct TransformNoOrientation {
    pub position: Point3<f32>,
    pub scale: (f32, f32, f32),
}
impl Default for TransformNoOrientation {
    fn default() -> Self {
        Self {
            position: (0.0, 0.0, 0.0).into(),
            scale: (1.0, 1.0, 1.0).into(),
        }
    }
}

impl From<&TransformNoOrientation> for Matrix4<f32> {
    fn from(v: &TransformNoOrientation) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.0, v.scale.1, v.scale.2);

        translation_matrix * scale_matrix
    }
}

impl From<&TransformNoOrientation> for [[f32; 4]; 4] {
    fn from(v: &TransformNoOrientation) -> Self {
        let translation_matrix = Matrix4::from_translation(v.position.to_vec());
        let scale_matrix = Matrix4::from_nonuniform_scale(v.scale.0, v.scale.1, v.scale.2);

        (translation_matrix * scale_matrix).into()
    }
}
