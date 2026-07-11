use crate::scene::types::{ObjectID, PointLight};
use anyhow::anyhow;

pub trait LightOperationListener {
    fn on_mod_point_light(&mut self, light: &PointLight, id: ObjectID);
    fn on_insert_point_light(&mut self, light: &PointLight, id: ObjectID);
    fn on_drop_point_light(&mut self, id: ObjectID);
}
pub struct LightHandler {
    point_lights: Vec<PointLight>,

    mod_pl_operations: Vec<ObjectID>,
    drop_pl_operations: Vec<ObjectID>,
    insert_pl_operations: Vec<ObjectID>,
}

impl LightHandler {
    pub fn new() -> Self {
        Self {
            point_lights: Vec::new(),
            mod_pl_operations: Vec::new(),
            drop_pl_operations: Vec::new(),
            insert_pl_operations: Vec::new(),
        }
    }
    pub fn insert_point_light(&mut self, light: PointLight) -> ObjectID {
        self.point_lights.push(light);
        let id = self.point_lights.len() - 1;
        self.insert_pl_operations.push(id);
        id
    }

    pub fn get(&self, id: ObjectID) -> Option<&PointLight> {
        Some(self.point_lights.get(id)?)
    }
    pub fn modify(
        &mut self,
        id: ObjectID,
        modify: impl FnOnce(&mut PointLight),
    ) -> anyhow::Result<()> {
        let light = self
            .point_lights
            .get_mut(id)
            .ok_or_else(|| anyhow!("Failed to get point light!"))?;
        modify(light);
        self.mod_pl_operations.push(id);
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
                .get(*id)
                .expect("Tried to update value on removed point light!");
            listener.on_insert_point_light(light, *id);
        }

        for id in self.mod_pl_operations.iter() {
            let light = self
                .point_lights
                .get(*id)
                .expect("Tried to update value on removed point light!");
            listener.on_mod_point_light(light, *id);
        }
    }
    pub fn clear_operations(&mut self) {
        self.mod_pl_operations.clear();
        self.insert_pl_operations.clear();
        self.drop_pl_operations.clear();
    }
}
