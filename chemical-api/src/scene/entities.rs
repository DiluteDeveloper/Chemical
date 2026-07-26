use crate::scene::types::{Mesh, Transform};

pub type EntityID = usize;

pub struct StaticMeshEntityDescriptor {
    pub transform: Transform,
    pub mesh: Mesh,
}

pub struct StaticMeshEntityRef<'a> {
    pub transform: &'a mut Transform,
    pub mesh: &'a Mesh,
}
