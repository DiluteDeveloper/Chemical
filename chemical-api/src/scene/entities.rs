pub mod static_mesh_entity;
pub use static_mesh_entity::StaticMeshEntity;

pub type EntityID = usize;

pub(crate) trait EntityList {
    type InsertType;
    type ReturnType;
    fn insert(&mut self, mesh: Self::InsertType) -> EntityID;
    fn get(&mut self, id: EntityID) -> Self::ReturnType;
}
