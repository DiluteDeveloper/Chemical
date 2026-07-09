use super::PointLight;
use anyhow::bail;

pub struct LightStorage {
    buffer: wgpu::Buffer,
    pub point_lights: Vec<PointLight>,
}

const MAX_POINT_LIGHTS: usize = 100;

type PointLightID = usize;

impl LightStorage {
    pub fn new(device: &wgpu::Device) -> Self {
        let size = size_of::<u32>() + (size_of::<PointLight>() * MAX_POINT_LIGHTS);
        let buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("light_storage_buffer"),
            size: size as u64,
            usage: wgpu::BufferUsages::STORAGE | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        LightStorage {
            buffer: buffer,
            point_lights: Vec::new(),
        }
    }

    pub fn add_point_light(&mut self, point_light: &PointLight) -> anyhow::Result<PointLightID> {
        if self.point_lights.len() == MAX_POINT_LIGHTS {
            bail!("Max number of point lights reached!")
        }
        self.point_lights.push(*point_light);
        Ok(self.point_lights.len() - 1)
    }

    pub(in crate::renderer::mesh_renderer) fn get_buffer(&self) -> &wgpu::Buffer {
        &self.buffer
    }
    pub(in crate::renderer::mesh_renderer) fn update_buffer(&mut self, queue: &wgpu::Queue) {
        queue.write_buffer(
            &self.buffer,
            0,
            bytemuck::bytes_of(&(self.point_lights.len() as u32)),
        );
        // point light len is bytes 0..15 because of padding
        queue.write_buffer(
            &self.buffer,
            size_of::<[f32; 4]>() as u64,
            bytemuck::cast_slice(&self.point_lights),
        );
    }
}
