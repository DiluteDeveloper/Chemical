mod shader_directional_light;
mod shader_point_light;
mod shadow_renderer;

use chemical_engine::scene::{
    type_handlers::light_handler::LightOperationListener,
    types::{DirectionalLight, EntityID, PointLight},
};
use log::warn;
use shader_directional_light::ShaderDirectionalLight;
use shadow_renderer::ShadowRenderer;

pub struct LightRenderer {
    directional_light_count_buffer: wgpu::Buffer,
    directional_lights_buffer: wgpu::Buffer,
    queue: wgpu::Queue,
    num_directional_lights: u32,
    pub shadow_renderer: ShadowRenderer,

    directional_light_alignment: u64,
}

impl LightRenderer {
    pub const MAX_DIRECTIONAL_LIGHTS: u64 = 256;

    pub fn new(
        model_matrix_bind_group_layout: &wgpu::BindGroupLayout,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
    ) -> Self {
        // This buffer is indexed by shadow renderer to get the projection,
        // but the whole buffer is used for each drawcall in the mesh renderer
        let directional_light_alignment: u64 = wgpu::util::align_to(
            size_of::<ShaderDirectionalLight>() as u64,
            device.limits().min_storage_buffer_offset_alignment as u64,
        );

        let directional_light_count_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("directional_light_count_buffer"),
            size: size_of::<f32>() as u64,
            usage: wgpu::BufferUsages::STORAGE | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let directional_lights_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("directional_lights_buffer"),
            size: directional_light_alignment * Self::MAX_DIRECTIONAL_LIGHTS,
            usage: wgpu::BufferUsages::STORAGE | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        LightRenderer {
            shadow_renderer: ShadowRenderer::new(model_matrix_bind_group_layout, device, queue),
            directional_lights_buffer,
            directional_light_count_buffer,
            queue: queue.clone(),
            num_directional_lights: 0,
            directional_light_alignment,
        }
    }

    pub fn get_directional_lights_buffer(&self) -> &wgpu::Buffer {
        &self.directional_lights_buffer
    }
    pub fn get_directional_light_count_buffer(&self) -> &wgpu::Buffer {
        &self.directional_light_count_buffer
    }

    pub fn start_queued_directional_light_shadow_pass<'a>(
        &'a mut self,
        encoder: &'a mut wgpu::CommandEncoder,
    ) -> Option<wgpu::RenderPass<'a>> {
        self.shadow_renderer
            .start_queued_directional_light_shadow_pass(encoder)
    }
}

impl LightOperationListener for LightRenderer {
    fn on_drop_directional_light(&mut self, _id: EntityID) {}
    fn on_drop_point_light(&mut self, _id: EntityID) {}
    fn on_insert_directional_light(&mut self, light: &DirectionalLight, id: EntityID) {
        if self.num_directional_lights + 1 >= Self::MAX_DIRECTIONAL_LIGHTS as u32 {
            warn!("Max number of directional lights reached! no more will be rendered.");
            return;
        }
        self.queue.write_buffer(
            &self.directional_light_count_buffer,
            0,
            bytemuck::bytes_of(&(self.num_directional_lights + 1)),
        );
        let shader_light: ShaderDirectionalLight = light.into();
        self.queue.write_buffer(
            &self.directional_lights_buffer,
            size_of::<ShaderDirectionalLight>() as u64 * (self.num_directional_lights as u64),
            bytemuck::bytes_of(&shader_light),
        );
        self.num_directional_lights += 1;
        self.shadow_renderer
            .add_directional_light_projection(&shader_light.projection);
    }
    fn on_insert_point_light(&mut self, _light: &PointLight, _id: EntityID) {}
    fn on_mod_directional_light(&mut self, light: &DirectionalLight, id: EntityID) {
        let shader_light: ShaderDirectionalLight = light.into();
        self.queue.write_buffer(
            &self.directional_lights_buffer,
            size_of::<ShaderDirectionalLight>() as u64 * (id as u64),
            bytemuck::bytes_of(&shader_light),
        );
        self.shadow_renderer
            .update_directional_light_projection(id, &shader_light.projection);
    }
    fn on_mod_point_light(&mut self, _light: &PointLight, _id: EntityID) {}
}
