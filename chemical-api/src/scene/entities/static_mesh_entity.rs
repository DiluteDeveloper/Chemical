use super::EntityID;
use super::EntityList;
use crate::scene::types::{StaticMesh, Transform};

// pub struct StaticMeshEntity<'a> {
//     pub transform: &'a mut Transform,
// }

pub struct StaticMeshEntityList {
    meshes: Vec<StaticMesh>,
    transforms: Vec<Transform>,
    last_checked_idx: usize,
}

impl Default for StaticMeshEntityList {
    fn default() -> Self {
        Self {
            meshes: Vec::new(),
            transforms: Vec::new(),
            last_checked_idx: 0,
        }
    }
}

impl<'a> EntityList for StaticMeshEntityList<'a> {
    type InsertType = (StaticMesh, Transform);
    type ReturnType = Option<&'a mut Transform>;
    fn insert(&mut self, value: Self::InsertType) -> EntityID {
        let idx = self.entities.len();
        self.meshes.push(value.0);
        self.transforms.push(value.1);
        idx
    }
    fn get(&mut self, id: EntityID) -> Self::ReturnType {
        Some(self.transforms.get_mut(id)?)
    }
}
// impl<'a> StaticMeshEntityList<'a> {
//     fn check_mesh_insertions(&mut self) -> Option<impl Iterator<Item = &StaticMesh>> {
//         let v = self.meshes.get(self.last_checked_idx..)?.iter();
//
//         self.last_checked_idx = self.meshes.len();
//
//         Some(v)
//     }
//     fn get_transforms(&self) -> impl Iterator<Item = &Transform> {
//         self.transforms.iter()
//     }
// }
