use crate::geometry::vertex::Vertex;
use crate::rendering::renderer::{self, Renderer};

pub struct IndexMesh {
    vertex_buffer: wgpu::Buffer,
    index_buffer: wgpu::Buffer,
    indices_len: u32,
    pub num_instances: u32,
}

#[allow(dead_code)]
pub struct VertexMesh {
    vertex_buffer: wgpu::Buffer,
    vertices_len: u32,
    pub num_instances: u32,
}

use wgpu::RenderPass;
use wgpu::util::DeviceExt;

impl IndexMesh {
    pub fn new(vertices: &[Vertex], indices: &[u16], renderer: &Renderer) -> Self {
        Self {
            vertex_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Vertex Buffer"),
                    contents: bytemuck::cast_slice(vertices),
                    usage: wgpu::BufferUsages::VERTEX,
                }),
            index_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Index Buffer"),
                    contents: bytemuck::cast_slice(indices),
                    usage: wgpu::BufferUsages::INDEX,
                }),
            indices_len: indices.len() as u32,
            num_instances: 1,
        }
    }
    pub fn new_instanced(
        vertices: &[Vertex],
        indices: &[u16],
        num_instances: u32,
        renderer: &Renderer,
    ) -> Self {
        Self {
            vertex_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Vertex Buffer"),
                    contents: bytemuck::cast_slice(vertices),
                    usage: wgpu::BufferUsages::VERTEX,
                }),
            index_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Index Buffer"),
                    contents: bytemuck::cast_slice(indices),
                    usage: wgpu::BufferUsages::INDEX,
                }),
            indices_len: indices.len() as u32,
            num_instances: num_instances,
        }
    }
}
impl VertexMesh {
    pub fn new(vertices: &[Vertex], renderer: &Renderer) -> Self {
        Self {
            vertex_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Vertex Buffer"),
                    contents: bytemuck::cast_slice(vertices),
                    usage: wgpu::BufferUsages::VERTEX,
                }),
            vertices_len: vertices.len() as u32,
            num_instances: 1,
        }
    }
    pub fn new_instanced(vertices: &[Vertex], num_instances: u32, renderer: &Renderer) -> Self {
        Self {
            vertex_buffer: renderer
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Vertex Buffer"),
                    contents: bytemuck::cast_slice(vertices),
                    usage: wgpu::BufferUsages::VERTEX,
                }),
            vertices_len: vertices.len() as u32,
            num_instances: num_instances,
        }
    }
}

impl renderer::Renderable for IndexMesh {
    fn bind(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        render_pass.set_index_buffer(self.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
    }
    fn draw(&self, render_pass: &mut RenderPass) {
        render_pass.draw_indexed(0..self.indices_len, 0, 0..self.num_instances);
    }
}
impl renderer::Renderable for VertexMesh {
    fn bind(&self, render_pass: &mut RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
    }
    fn draw(&self, render_pass: &mut RenderPass) {
        render_pass.draw(0..self.vertices_len, 0..self.num_instances);
    }
}
