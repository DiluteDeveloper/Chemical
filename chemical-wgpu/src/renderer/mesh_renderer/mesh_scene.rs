use super::{IndexMesh, VertexMesh};
use crate::utility::Transform;
pub struct MeshScene {
    pub lit_vertex_meshes: Vec<(VertexMesh, TransformID)>,
    pub lit_index_meshes: Vec<(IndexMesh, TransformID)>,

    pub unlit_vertex_meshes: Vec<(VertexMesh, TransformID)>,
    pub unlit_index_meshes: Vec<(IndexMesh, TransformID)>,

    pub transforms: Vec<Transform>,

    pub aligned_model_matrix_size_offset: u64,
}

impl MeshScene {
    pub fn new(aligned_model_matrix_size_offset: u64) -> Self {
        Self {
            lit_vertex_meshes: Vec::new(),
            lit_index_meshes: Vec::new(),
            unlit_vertex_meshes: Vec::new(),
            unlit_index_meshes: Vec::new(),
            transforms: Vec::new(),
            aligned_model_matrix_size_offset: aligned_model_matrix_size_offset,
        }
    }
}

type TransformID = usize;
