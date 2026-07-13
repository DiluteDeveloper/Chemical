use super::type_handlers::{LightHandler, MeshHandler, TransformHandler};
pub struct SceneContainer {
    pub transform_handler: TransformHandler,
    pub light_handler: LightHandler,
    pub mesh_handler: MeshHandler,
}

// Need to develop a system of having a vector of ObjectIDs,
// and when one is dropped, leave the space open and record all the open spaces
// to be able to insert new elements in
impl SceneContainer {
    pub fn new() -> Self {
        Self {
            transform_handler: TransformHandler::new(),
            mesh_handler: MeshHandler::new(),
            light_handler: LightHandler::new(),
        }
    }

    pub fn clear_operations(&mut self) {
        self.transform_handler.clear_operations();
        self.light_handler.clear_operations();
        self.mesh_handler.clear_operations();
    }
}
