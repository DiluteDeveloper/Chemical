#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
}
pub type Index = u32;

#[derive(Clone)]
pub struct StaticMesh {
    pub vertices: Vec<Vertex>,
    pub indices: Option<Vec<Index>>,
    pub is_lit: bool,
}

impl StaticMesh {
    pub fn new(is_lit: bool) -> Self {
        StaticMesh {
            vertices: Vec::new(),
            indices: None,
            is_lit,
        }
    }
    pub fn new_with_data(vertices: &[Vertex], indices: Option<&[Index]>, is_lit: bool) -> Self {
        StaticMesh {
            vertices: vertices.to_vec(),
            indices: indices.map(|indices| indices.to_vec()),
            is_lit,
        }
    }
}
