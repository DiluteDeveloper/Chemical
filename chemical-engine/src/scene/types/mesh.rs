#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
}
pub type Index = u32;

#[derive(Clone)]
pub struct Mesh {
    pub vertices: Vec<Vertex>,
    pub indices: Option<Vec<Index>>,
    pub is_lit: bool,
}

impl Mesh {
    pub fn new(vertices: &[Vertex], indices: Option<&[Index]>, is_lit: bool) -> Self {
        Mesh {
            vertices: vertices.to_vec(),
            indices: indices.map(|indices| indices.to_vec()),
            is_lit,
        }
    }
}
