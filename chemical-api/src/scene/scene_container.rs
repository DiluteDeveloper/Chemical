use super::EntityID;
use super::TrackedEntityList;

pub trait HasEntityList<T> {
    fn get_entity_list(&self) -> &TrackedEntityList<T>;
    fn get_entity_list_mut(&mut self) -> &mut TrackedEntityList<T>;
}

macro_rules! define_scene{
    ($($field:ident : $ty:ty),* $(,)?) => {
        #[derive(Default)]
        pub struct Scene{
            $($field: TrackedEntityList<$ty>),*
        }
        $(
            impl HasEntityList<$ty> for Scene {
                fn get_entity_list(&self) -> &TrackedEntityList<$ty> {
                    &self.$field
                }
                fn get_entity_list_mut(&mut self) -> &mut TrackedEntityList<$ty> {
                    &mut self.$field
                }
            }
        )*
    };
}

define_scene! {
    static_meshes: super::entities::StaticMeshEntity,
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
