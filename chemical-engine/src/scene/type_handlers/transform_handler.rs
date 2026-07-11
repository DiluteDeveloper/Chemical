use crate::scene::types::{EntityID, Transform};
use anyhow::anyhow;
use std::collections::HashMap;

pub trait TransformOperationListener {
    fn on_mod(&mut self, transform: &Transform, id: EntityID);
    fn on_insert(&mut self, transform: &Transform, id: EntityID);
    fn on_drop(&mut self, id: EntityID);
}
pub struct TransformHandler {
    transforms: HashMap<EntityID, Transform>,

    mod_operations: Vec<EntityID>,
    drop_operations: Vec<EntityID>,
    insert_operations: Vec<EntityID>,
}

impl TransformHandler {
    pub fn new() -> Self {
        Self {
            transforms: HashMap::new(),
            mod_operations: Vec::new(),
            drop_operations: Vec::new(),
            insert_operations: Vec::new(),
        }
    }
    pub fn insert_transform(&mut self, transform: Transform, id: EntityID) {
        self.transforms.insert(id, transform);
        self.insert_operations.push(id);
    }

    pub fn get(&self, id: EntityID) -> Option<&Transform> {
        Some(self.transforms.get(&id)?)
    }
    pub fn modify(
        &mut self,
        id: EntityID,
        modify: impl FnOnce(&mut Transform),
    ) -> anyhow::Result<()> {
        let transform = self
            .transforms
            .get_mut(&id)
            .ok_or_else(|| anyhow!("Failed to get transform!"))?;
        modify(transform);
        self.mod_operations.push(id);
        Ok(())
    }

    pub fn dispatch_operations(&self, listener: &mut impl TransformOperationListener) {
        // I believe drop, insert, and mod operations need to be done in this order
        for id in self.drop_operations.iter() {
            listener.on_drop(*id);
        }
        for id in self.insert_operations.iter() {
            let transform = self
                .transforms
                .get(id)
                .expect("Tried to update value on removed transform!");
            listener.on_insert(transform, *id);
        }

        for id in self.mod_operations.iter() {
            let transform = self
                .transforms
                .get(id)
                .expect("Tried to update value on removed transform!");
            listener.on_mod(transform, *id);
        }
    }
    pub fn clear_operations(&mut self) {
        self.mod_operations.clear();
        self.insert_operations.clear();
        self.drop_operations.clear();
    }
}
