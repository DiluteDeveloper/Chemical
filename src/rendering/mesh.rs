use crate::geometry::vertex::Vertex;

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
    fn bind(&self, render_pass: &mut RenderPass);
    fn draw(&self, render_pass: &mut RenderPass, num_instances: u32);
}

impl Renderable for IndexMesh {
    fn bind(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        render_pass.set_index_buffer(self.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
    }
    fn draw(&self, render_pass: &mut RenderPass, num_instances: u32) {
        render_pass.draw_indexed(0..self.indices_len, 0, 0..num_instances);
    }
}
impl Renderable for VertexMesh {
    fn bind(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
    }
    fn draw(&self, render_pass: &mut RenderPass, num_instances: u32) {
        render_pass.draw(0..self.vertices_len, 0..num_instances);
    }
}
