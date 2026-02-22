use crate::geometry::vertex::Vertex;

//pub const VERTICES: &[vertex::Vertex] = &[
//    // Changed
//    Vertex {
//        position: [-0.0868241, 0.49240386, 0.0],
//        ..
//    }, // A
//    Vertex {
//        position: [-0.49513406, 0.06958647, 0.0],
//        tex_coords: [0.0048659444, 0.43041354],
//    }, // B
//    Vertex {
//        position: [-0.21918549, -0.44939706, 0.0],
//        tex_coords: [0.28081453, 0.949397],
//    }, // C
//    Vertex {
//        position: [0.35966998, -0.3473291, 0.0],
//        tex_coords: [0.85967, 0.84732914],
//    }, // D
//    Vertex {
//        position: [0.44147372, 0.2347359, 0.0],
//        tex_coords: [0.9414737, 0.2652641],
//    }, // E
//];
//
//pub const INDICES: &[u16] = &[0, 1, 4, 1, 2, 4, 2, 3, 4];

pub struct IndexMesh {
    vertex_buffer: wgpu::Buffer,
    index_buffer: wgpu::Buffer,
    indices_len: u32,
}

#[allow(dead_code)]
pub struct VertexMesh {
    vertex_buffer: wgpu::Buffer,
    vertices_len: u32,
}

use wgpu::RenderPass;
use wgpu::util::DeviceExt;

impl IndexMesh {
    pub fn new(vertices: &[Vertex], indices: &[u16], device: &wgpu::Device) -> Self {
        Self {
            vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: Some("Vertex Buffer"),
                contents: bytemuck::cast_slice(vertices),
                usage: wgpu::BufferUsages::VERTEX,
            }),
            index_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: Some("Index Buffer"),
                contents: bytemuck::cast_slice(indices),
                usage: wgpu::BufferUsages::INDEX,
            }),
            indices_len: indices.len() as u32,
        }
    }
}
impl VertexMesh {
    #[allow(unused)]
    pub fn new(vertices: &[Vertex], device: &wgpu::Device) -> Self {
        Self {
            vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: Some("Vertex Buffer"),
                contents: bytemuck::cast_slice(vertices),
                usage: wgpu::BufferUsages::VERTEX,
            }),
            vertices_len: vertices.len() as u32,
        }
    }
}

pub trait Renderable {
    fn render(&self, render_pass: &mut RenderPass);
}

impl Renderable for IndexMesh {
    fn render(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        render_pass.set_index_buffer(self.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
        render_pass.draw_indexed(0..self.indices_len, 0, 0..1);
    }
}
impl Renderable for VertexMesh {
    fn render(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        render_pass.draw(0..self.vertices_len, 0..1);
    }
}
