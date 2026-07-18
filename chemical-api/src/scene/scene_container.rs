use super::entities::{EntityID, EntityList};

pub trait HasEntityList<T: EntityList> {
    fn get_entity_list(&self) -> &T;
    fn get_entity_list_mut(&mut self) -> &mut T;
}

macro_rules! define_scene{
    ($($field:ident : $ty:ty),* $(,)?) => {
        #[derive(Default)]
        pub struct Scene{
            $($field: $ty),*
        }
        $(
            impl HasEntityList<$ty> for Scene {
                fn get_entity_list(&self) -> &$ty {
                    &self.$field
                }
                fn get_entity_list_mut(&mut self) -> &mut $ty {
                    &mut self.$field
                }
            }
        )*
    };
}

define_scene! {
    static_meshes: super::StaticMeshEntity,
}
impl Scene {
    pub fn insert_entity<Entity>(&mut self, entity: Entity) -> EntityID
    where
        Self: HasEntityList<Entity>,
    {
        self.get_entity_list_mut().insert(entity)
    }
    pub fn get_entity<Entity>(&mut self, id: EntityID) -> Option<&mut Entity>
    where
        Self: HasEntityList<Entity>,
    {
        self.get_entity_list_mut().get(id)
    }
    pub(crate) fn get_tracked_entity_list<Entity>(&mut self) -> &TrackedEntityList<Entity>
    where
        Self: HasEntityList<Entity>,
    {
        self.get_entity_list()
    }
}
