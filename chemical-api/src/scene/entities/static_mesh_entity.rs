use crate::scene::types::{StaticMesh, Transform};
pub struct StaticMeshEntity {
    pub transform: Transform,
    static_mesh: StaticMesh,
}

impl StaticMeshEntity {
    pub fn new(transform: Transform, static_mesh: StaticMesh) -> Self {
        Self {
            transform,
            static_mesh,
        }
    }
}
