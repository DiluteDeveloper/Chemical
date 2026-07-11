#[repr(C)]
#[derive(Clone, Copy, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
}
pub struct VertexMesh {
    pub vertices: Vec<Vertex>,
    pub num_instances: u32,
    pub is_lit: bool,
}

impl VertexMesh {
    pub fn new(vertices: &[Vertex], num_instances: u32, is_lit: bool) -> Self {
        VertexMesh {
            vertices: vertices.to_vec(),
            num_instances,
            is_lit,
        }
    }
}
