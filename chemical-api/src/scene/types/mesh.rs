#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
}
pub type Index = u32;

#[derive(Clone)]
pub struct StaticMesh {
    pub(crate) vertices: Vec<Vertex>,
    pub(crate) indices: Option<Vec<Index>>,
    pub(crate) is_lit: bool,
}

impl StaticMesh {
    pub fn new(vertices: &[Vertex], indices: Option<&[Index]>, is_lit: bool) -> Self {
        StaticMesh {
            vertices: vertices.to_vec(),
            indices: indices.map(|indices| indices.to_vec()),
            is_lit,
        }
    }
}
