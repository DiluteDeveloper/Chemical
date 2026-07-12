use crate::scene::types::{DirectionalLight, EntityID, PointLight};
use anyhow::anyhow;
use std::collections::HashMap;

pub trait LightOperationListener {
    fn on_mod_point_light(&mut self, light: &PointLight, id: EntityID);
    fn on_insert_point_light(&mut self, light: &PointLight, id: EntityID);
    fn on_drop_point_light(&mut self, id: EntityID);

    fn on_mod_directional_light(&mut self, light: &DirectionalLight, id: EntityID);
    fn on_insert_directional_light(&mut self, light: &DirectionalLight, id: EntityID);
    fn on_drop_directional_light(&mut self, id: EntityID);
}
pub struct LightHandler {
    point_lights: HashMap<EntityID, PointLight>,

    mod_pl_operations: Vec<EntityID>,
    drop_pl_operations: Vec<EntityID>,
    insert_pl_operations: Vec<EntityID>,

    directional_lights: HashMap<EntityID, DirectionalLight>,

    mod_dl_operations: Vec<EntityID>,
    drop_dl_operations: Vec<EntityID>,
    insert_dl_operations: Vec<EntityID>,
}

impl LightHandler {
    pub fn new() -> Self {
        Self {
            point_lights: HashMap::new(),
            mod_pl_operations: Vec::new(),
            drop_pl_operations: Vec::new(),
            insert_pl_operations: Vec::new(),
            directional_lights: HashMap::new(),
            mod_dl_operations: Vec::new(),
            drop_dl_operations: Vec::new(),
            insert_dl_operations: Vec::new(),
        }
    }
    pub fn insert_point_light(&mut self, light: PointLight, id: EntityID) {
        self.point_lights.insert(id, light);
        self.insert_pl_operations.push(id);
    }
    pub fn insert_directional_light(&mut self, light: DirectionalLight, id: EntityID) {
        self.directional_lights.insert(id, light);
        self.insert_dl_operations.push(id);
    }

    pub fn get_point_light(&self, id: EntityID) -> Option<&PointLight> {
        Some(self.point_lights.get(&id)?)
    }
    pub fn get_directional_light(&self, id: EntityID) -> Option<&DirectionalLight> {
        Some(self.directional_lights.get(&id)?)
    }
    pub fn modify_point_light(
        &mut self,
        id: EntityID,
        modify: impl FnOnce(&mut PointLight),
    ) -> anyhow::Result<()> {
        let light = self
            .point_lights
            .get_mut(&id)
            .ok_or_else(|| anyhow!("Failed to get point light!"))?;
        modify(light);
        self.mod_pl_operations.push(id);
        Ok(())
    }
    pub fn modify_directional_light(
        &mut self,
        id: EntityID,
        modify: impl FnOnce(&mut DirectionalLight),
    ) -> anyhow::Result<()> {
        let light = self
            .directional_lights
            .get_mut(&id)
            .ok_or_else(|| anyhow!("Failed to get directional light!"))?;
        modify(light);
        self.mod_dl_operations.push(id);
        Ok(())
    }

    pub fn dispatch_operations(&self, listener: &mut impl LightOperationListener) {
        // I believe drop, insert, and mod operations need to be done in this order
        for id in self.drop_pl_operations.iter() {
            listener.on_drop_point_light(*id);
        }
        for id in self.insert_pl_operations.iter() {
            let light = self
                .point_lights
                .get(id)
                .expect("Tried to update value on removed point light!");
            listener.on_insert_point_light(light, *id);
        }

        for id in self.mod_pl_operations.iter() {
            let light = self
                .point_lights
                .get(id)
                .expect("Tried to update value on removed point light!");
            listener.on_mod_point_light(light, *id);
        }
        for id in self.drop_dl_operations.iter() {
            listener.on_drop_directional_light(*id);
        }
        for id in self.insert_dl_operations.iter() {
            let light = self
                .directional_lights
                .get(id)
                .expect("Tried to update value on removed directional light!");
            listener.on_insert_directional_light(light, *id);
        }

        for id in self.mod_dl_operations.iter() {
            let light = self
                .directional_lights
                .get(id)
                .expect("Tried to update value on removed directional light!");
            listener.on_mod_directional_light(light, *id);
        }
    }
    pub fn clear_operations(&mut self) {
        self.mod_pl_operations.clear();
        self.insert_pl_operations.clear();
        self.drop_pl_operations.clear();
        self.mod_dl_operations.clear();
        self.insert_dl_operations.clear();
        self.drop_dl_operations.clear();
    }
}
