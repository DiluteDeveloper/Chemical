use super::RenderPointLight;
use chemical_engine::scene::types::{EntityID, PointLight};
pub struct LightStorage {
    pub buffer: wgpu::Buffer,
    queue: wgpu::Queue,
    len: u32,
}

const MAX_POINT_LIGHTS: usize = 100;

impl LightStorage {
    pub fn new(device: &wgpu::Device, queue: &wgpu::Queue) -> Self {
        let size = size_of::<u32>() + (size_of::<RenderPointLight>() * MAX_POINT_LIGHTS);
        let buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("light_storage_buffer"),
            size: size as u64,
            usage: wgpu::BufferUsages::STORAGE | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        LightStorage {
            buffer: buffer,
            queue: queue.clone(),
            len: 0,
        }
    }

    pub(in crate::renderer::mesh_renderer) fn on_insert_point_light(
        &mut self,
        light: &PointLight,
        id: EntityID,
    ) {
        self.len += 1;
        // point light len is bytes 0..15 because of padding; be explicit here
        self.queue
            .write_buffer(&self.buffer, 0, bytemuck::bytes_of(&(self.len as u32)));

        let rpl: RenderPointLight = light.into();
        self.queue.write_buffer(
            &self.buffer,
            size_of::<[f32; 4]>() as u64 + (size_of::<RenderPointLight>() * id as usize) as u64,
            bytemuck::bytes_of(&rpl),
        );
    }
}
