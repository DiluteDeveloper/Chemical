use super::{
    component_sparse_set::ComponentSparseSet,
    entities::EntityID,
    types::{Mesh, Transform},
};

trait HasComponentSparseSet<T> {
    fn insert(&mut self, component: T, id: EntityID);
    fn get(&mut self) -> impl Iterator<Item = T>;
}

#[allow(unused_macros)]
macro_rules! define_scene{
    ($($field:ident : $ty:ty),* $(,)?) => {
        #[derive(Default)]
        pub struct Scene{
            $($field: ComponentSparseSet<$ty>),*
        }
        $(
            impl HasComponentSparseSet<$ty> for Scene {
                fn insert(&mut self, component: $ty, id: EntityID) {
                    self.$field.insert(component, id);
                }
                fn get(&mut self) -> impl Iterator<Item = $ty> {
                    self.$field.get()
                }
                // fn get_entity_list(&self) -> &$ty {
                //     &self.$field
                // }
                // fn get_entity_list_mut(&mut self) -> &mut $ty {
                //     &mut self.$field
                // }
            }
        )*
    };
}
define_scene!(
    static_meshes: Mesh,
    transforms: Transform,
);

impl Scene {
    pub fn get<A>(&mut self) -> impl Iterator<Item = A>
    where
        Self: HasComponentSparseSet<A>,
    {
        self.get()
    }
    pub fn get<A, B>(&mut self) -> impl Iterator<Item = (A, B)>
    where
        Self: HasComponentSparseSet<A>,
        Self: HasComponentSparseSet<B>,
    {
        self.get()
    }
}
//     static_meshes: Vec<Mesh>,
//     transforms: Vec<Transform>,
//
//     static_meshes_sparse: HashMap<EntityID, usize>,
//     transforms_sparse: HashMap<EntityID, usize>,
// }
// impl Scene {
//     pub fn insert_static_mesh_entity(&mut self, entity: StaticMeshEntityDescriptor, id: EntityID) {
//         self.static_meshes_sparse
//             .insert(id, self.static_meshes_dense.len());
//         self.static_meshes.push(entity.static_mesh);
//         self.static_mesh_transforms_dense.push(entity.transform);
//     }
//     pub fn get_static_mesh_entity(&mut self, id: EntityID) -> Option<StaticMeshEntityRef> {
//         let i = self.static_meshes_sparse.get(&id)?;
//         Some(StaticMeshEntityRef {
//             transform: self.static_mesh_transforms_dense.get_mut(i)?,
//             mesh: self.static_meshes_dense.get(i)?,
//         })
//     }
//     pub(crate) fn check_inserted_static_meshes(&self) -> Option<impl Iterator<Item = Mesh>> {
//         let v = self
//             .static_meshes_dense
//             .get(self.next_unchecked_static_mesh_idx..)?
//             .iter();
//
//         self.next_unchecked_static_mesh_idx = self.static_meshes_dense.len();
//
//         Some(v)
//     }
// }
