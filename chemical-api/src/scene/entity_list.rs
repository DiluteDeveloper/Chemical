use super::EntityID;

pub struct TrackedEntityList<Entity> {
    entities: Vec<Entity>,
    last_checked_idx: usize,
}

impl<Entity> Default for TrackedEntityList<Entity> {
    fn default() -> Self {
        Self {
            entities: Vec::new(),
            last_checked_idx: 0,
        }
    }
}

impl<Entity> TrackedEntityList<Entity> {
    pub fn insert(&mut self, entity: Entity) -> EntityID {
        let idx = self.entities.len();
        self.entities.push(entity);
        idx
    }
    pub fn get(&mut self, id: EntityID) -> Option<&mut Entity> {
        self.entities.get_mut(id)
    }

    pub(crate) fn check_insertions(&mut self) -> Option<impl Iterator<Item = &Entity>> {
        let v = self.entities.get(self.last_checked_idx..)?.iter();

        self.last_checked_idx = self.entities.len();

        Some(v)
    }
    pub(crate) fn get_entities(&self) -> impl Iterator<Item = &Entity> {
        self.entities.iter()
    }
}
