use super::ObjectID;
use super::vertex_mesh::Vertex;

pub type Index = u32;

pub struct IndexMesh {
    pub vertices: Vec<Vertex>,
    pub indices: Vec<Index>,
    pub num_instances: u32,
    pub is_lit: bool,
    pub transform_id: ObjectID,
}

impl IndexMesh {
    pub fn new(
        vertices: &[Vertex],
        indices: &[Index],
        num_instances: u32,
        is_lit: bool,
        transform_id: ObjectID,
    ) -> Self {
        IndexMesh {
            vertices: vertices.to_vec(),
            indices: indices.to_vec(),
            num_instances,
            is_lit,
            transform_id,
        }
    }
}
