use wgpu::util::DeviceExt;

use super::renderable::Renderable;

use chemical_engine::scene::types::{index_mesh::Index, vertex_mesh::Vertex};
pub(super) struct BakedIndexMesh {
    pub(super) vertex_buffer: wgpu::Buffer,
    pub(super) index_buffer: wgpu::Buffer,
    pub(super) len: u32,
    pub(super) num_instances: u32,
}
pub(super) struct BakedVertexMesh {
    pub(super) vertex_buffer: wgpu::Buffer,
    pub(super) len: u32,
    pub(super) num_instances: u32,
}

impl BakedIndexMesh {
    pub(super) fn new(
        vertices: &Vec<Vertex>,
        indices: &Vec<Index>,
        num_instances: u32,
        device: &wgpu::Device,
    ) -> Self {
        Self {
            vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: None,
                contents: bytemuck::cast_slice(&vertices),
                usage: wgpu::BufferUsages::VERTEX,
            }),
            index_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: None,
                contents: bytemuck::cast_slice(&indices),
                usage: wgpu::BufferUsages::INDEX,
            }),
            len: indices.len() as u32,
            num_instances: num_instances,
        }
    }
}
impl Renderable for BakedIndexMesh {
    fn bind(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        render_pass.set_index_buffer(self.index_buffer.slice(..), wgpu::IndexFormat::Uint32);
    }
    fn draw(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.draw_indexed(0..self.len, 0, 0..self.num_instances);
    }
}
impl BakedVertexMesh {
    pub(super) fn new(vertices: &Vec<Vertex>, num_instances: u32, device: &wgpu::Device) -> Self {
        Self {
            vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: None,
                contents: bytemuck::cast_slice(&vertices),
                usage: wgpu::BufferUsages::VERTEX,
            }),
            len: vertices.len() as u32,
            num_instances: num_instances,
        }
    }
}

impl Renderable for BakedVertexMesh {
    fn bind(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
    }
    fn draw(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.draw(0..self.len, 0..self.num_instances);
    }
}
