use crate::scene::types::{IndexMesh, ObjectID, VertexMesh};
use anyhow::anyhow;

pub trait MeshOperationListener {
    fn on_mod_index_mesh(&mut self, mesh: &IndexMesh, id: ObjectID);
    fn on_insert_index_mesh(&mut self, mesh: &IndexMesh, id: ObjectID);
    fn on_drop_index_mesh(&mut self, id: ObjectID);

    fn on_mod_vertex_mesh(&mut self, mesh: &VertexMesh, id: ObjectID);
    fn on_insert_vertex_mesh(&mut self, mesh: &VertexMesh, id: ObjectID);
    fn on_drop_vertex_mesh(&mut self, id: ObjectID);
}
pub struct MeshHandler {
    imeshes: Vec<IndexMesh>,
    vmeshes: Vec<VertexMesh>,

    mod_imesh_operations: Vec<ObjectID>,
    drop_imesh_operations: Vec<ObjectID>,
    insert_imesh_operations: Vec<ObjectID>,

    mod_vmesh_operations: Vec<ObjectID>,
    drop_vmesh_operations: Vec<ObjectID>,
    insert_vmesh_operations: Vec<ObjectID>,
}

impl MeshHandler {
    pub fn new() -> Self {
        Self {
            imeshes: Vec::new(),
            vmeshes: Vec::new(),
            mod_imesh_operations: Vec::new(),
            mod_vmesh_operations: Vec::new(),
            drop_imesh_operations: Vec::new(),
            drop_vmesh_operations: Vec::new(),
            insert_imesh_operations: Vec::new(),
            insert_vmesh_operations: Vec::new(),
        }
    }
    pub fn insert_imesh(&mut self, index_mesh: IndexMesh) -> ObjectID {
        self.imeshes.push(index_mesh);
        let id = self.imeshes.len() - 1;
        self.insert_imesh_operations.push(id);
        id
    }
    pub fn insert_vmesh(&mut self, vertex_mesh: VertexMesh) -> ObjectID {
        self.vmeshes.push(vertex_mesh);
        let id = self.vmeshes.len() - 1;
        self.insert_vmesh_operations.push(id);
        id
    }

    pub fn get_imesh(&self, id: ObjectID) -> Option<&IndexMesh> {
        Some(self.imeshes.get(id)?)
    }
    pub fn get_vmesh(&self, id: ObjectID) -> Option<&VertexMesh> {
        Some(self.vmeshes.get(id)?)
    }
    pub fn modify_vmesh(
        &mut self,
        id: ObjectID,
        modify: impl FnOnce(&mut VertexMesh),
    ) -> anyhow::Result<()> {
        let vmesh = self
            .vmeshes
            .get_mut(id)
            .ok_or_else(|| anyhow!("Failed to get transform!"))?;
        modify(vmesh);
        self.mod_vmesh_operations.push(id);
        Ok(())
    }
    pub fn modify_imesh(
        &mut self,
        id: ObjectID,
        modify: impl FnOnce(&mut IndexMesh),
    ) -> anyhow::Result<()> {
        let imesh = self
            .imeshes
            .get_mut(id)
            .ok_or_else(|| anyhow!("Failed to get transform!"))?;
        modify(imesh);
        self.mod_imesh_operations.push(id);
        Ok(())
    }
    pub fn dispatch_operations(&self, listener: &mut impl MeshOperationListener) {
        // I believe drop, insert, and mod operations need to be done in this order
        for id in self.drop_imesh_operations.iter() {
            listener.on_drop_index_mesh(*id);
        }
        for id in self.insert_imesh_operations.iter() {
            let mesh = self
                .imeshes
                .get(*id)
                .expect("Tried to update value on removed transform!");
            listener.on_insert_index_mesh(mesh, *id);
        }
        for id in self.mod_imesh_operations.iter() {
            let mesh = self
                .imeshes
                .get(*id)
                .expect("Tried to update value on removed transform!");
            listener.on_mod_index_mesh(mesh, *id);
        }

        for id in self.drop_vmesh_operations.iter() {
            listener.on_drop_vertex_mesh(*id);
        }
        for id in self.insert_vmesh_operations.iter() {
            let mesh = self
                .vmeshes
                .get(*id)
                .expect("Tried to update value on removed transform!");
            listener.on_insert_vertex_mesh(mesh, *id);
        }
        for id in self.mod_vmesh_operations.iter() {
            let mesh = self
                .vmeshes
                .get(*id)
                .expect("Tried to update value on removed transform!");
            listener.on_mod_vertex_mesh(mesh, *id);
        }
    }
    pub fn clear_operations(&mut self) {
        self.drop_imesh_operations.clear();
        self.drop_vmesh_operations.clear();
        self.insert_imesh_operations.clear();
        self.insert_vmesh_operations.clear();
        self.mod_imesh_operations.clear();
        self.mod_vmesh_operations.clear();
    }
}
