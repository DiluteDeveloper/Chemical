use std::collections::HashMap;

use anyhow::anyhow;

use crate::scene::types::{EntityID, Mesh};

pub trait MeshOperationListener {
    fn on_mod_mesh(&mut self, mesh: &Mesh, id: EntityID);
    fn on_insert_mesh(&mut self, mesh: &Mesh, id: EntityID);
    fn on_drop_mesh(&mut self, id: EntityID);
}
pub struct MeshHandler {
    meshes: HashMap<EntityID, Mesh>,

    mod_mesh_operations: Vec<EntityID>,
    drop_mesh_operations: Vec<EntityID>,
    insert_mesh_operations: Vec<EntityID>,
}

impl MeshHandler {
    pub fn new() -> Self {
        Self {
            meshes: HashMap::new(),
            mod_mesh_operations: Vec::new(),
            drop_mesh_operations: Vec::new(),
            insert_mesh_operations: Vec::new(),
        }
    }
    pub fn insert_mesh(&mut self, mesh: Mesh, id: EntityID) {
        self.meshes.insert(id, mesh);
        self.insert_mesh_operations.push(id);
    }

    pub fn get_mesh(&self, id: EntityID) -> Option<&Mesh> {
        Some(self.meshes.get(&id)?)
    }
    pub fn modify_mesh(
        &mut self,
        id: EntityID,
        modify: impl FnOnce(&mut Mesh),
    ) -> anyhow::Result<()> {
        let mesh = self
            .meshes
            .get_mut(&id)
            .ok_or_else(|| anyhow!("Failed to get mesh!"))?;
        modify(mesh);
        self.mod_mesh_operations.push(id);
        Ok(())
    }
    pub fn dispatch_operations(&self, listener: &mut impl MeshOperationListener) {
        // I believe drop, insert, and mod operations need to be done in this order
        for id in self.drop_mesh_operations.iter() {
            listener.on_drop_mesh(*id);
        }
        for id in self.insert_mesh_operations.iter() {
            let mesh = self
                .meshes
                .get(id)
                .expect("Tried to update value on removed mesh!");
            listener.on_insert_mesh(mesh, *id);
        }
        for id in self.mod_mesh_operations.iter() {
            let mesh = self
                .meshes
                .get(id)
                .expect("Tried to update value on removed mesh!");
            listener.on_mod_mesh(mesh, *id);
        }
    }
    pub fn clear_operations(&mut self) {
        self.drop_mesh_operations.clear();
        self.insert_mesh_operations.clear();
        self.mod_mesh_operations.clear();
    }
}
