use wgpu::util::DeviceExt;

use super::renderable::Renderable;
use crate::scene::types::{mesh::Index, mesh::Vertex};
#[derive(Debug)]
pub struct RenderedMesh {
    vertex_buffer: wgpu::Buffer,
    index_buffer: Option<wgpu::Buffer>,

    len: u32,
}

impl RenderedMesh {
    pub(super) fn new(
        vertices: &[Vertex],
        indices: Option<&[Index]>,
        device: &wgpu::Device,
    ) -> Self {
        Self {
            vertex_buffer: device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                label: Some("vertex_buffer"),
                contents: bytemuck::cast_slice(&vertices),
                usage: wgpu::BufferUsages::VERTEX,
            }),
            index_buffer: indices.map(|indices| {
                device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("index_buffer"),
                    contents: bytemuck::cast_slice(&indices),
                    usage: wgpu::BufferUsages::INDEX,
                })
            }),
            len: match indices {
                Some(indices) => indices.len() as u32,
                None => vertices.len() as u32,
            },
        }
    }
}
impl Renderable for RenderedMesh {
    fn bind(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_vertex_buffer(0, self.vertex_buffer.slice(..));
        if let Some(index_buffer) = &self.index_buffer {
            render_pass.set_index_buffer(index_buffer.slice(..), wgpu::IndexFormat::Uint32);
        }
    }
    fn draw(&self, render_pass: &mut wgpu::RenderPass) {
        match &self.index_buffer {
            Some(_i) => {
                render_pass.draw_indexed(0..self.len, 0, 0..1);
            }
            None => {
                render_pass.draw(0..self.len, 0..1);
            }
        }
    }
}
