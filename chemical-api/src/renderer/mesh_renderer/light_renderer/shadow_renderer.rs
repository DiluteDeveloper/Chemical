use std::num::NonZeroU64;

use super::super::geometry::vertex;
use crate::renderer::mesh_renderer::light_renderer::LightRenderer;

#[derive(Debug)]
pub struct ShadowRenderer {
    directional_light_maps: [wgpu::TextureView; LightRenderer::MAX_DIRECTIONAL_LIGHTS as usize],
    num_directional_lights: u32,
    render_pipeline: wgpu::RenderPipeline,

    directional_light_projection_buffer: wgpu::Buffer,
    directional_light_projection_bind_group: wgpu::BindGroup,

    queue: wgpu::Queue,
    device: wgpu::Device,

    // Index of directional light that needs to be recomputed
    queued_directional_light_shadow_passes: Vec<u32>,

    lightmap_sampler: wgpu::Sampler,
    matrix_uniform_offset: u64,
}

impl ShadowRenderer {
    const SHADOW_SHADER_PATH: &str = "res/shaders/shadow_shader.wgsl";

    pub fn new(
        model_matrix_bind_group_layout: &wgpu::BindGroupLayout,
        device: &wgpu::Device,
        queue: &wgpu::Queue,
    ) -> Self {
        let light_projection_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: true,
                        min_binding_size: Some(
                            NonZeroU64::new(size_of::<[[f32; 4]; 4]>() as u64).unwrap(),
                        ),
                    },
                    count: None,
                }],
                label: Some("shadow_renderer_light_projection_bind_group_layout"),
            });
        let shader_source = std::fs::read_to_string(Self::SHADOW_SHADER_PATH).expect(&format!(
            "Shader path '{}' was invalid!",
            Self::SHADOW_SHADER_PATH
        ));

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(Self::SHADOW_SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(shader_source.into()),
        });

        let matrix_uniform_offset: u64 = wgpu::util::align_to(
            size_of::<[[f32; 4]; 4]>() as u32,
            device.limits().min_uniform_buffer_offset_alignment,
        ) as u64;

        let directional_light_projection_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("shadow_renderer_directional_light_projection_buffer"),
            size: matrix_uniform_offset * LightRenderer::MAX_DIRECTIONAL_LIGHTS,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let directional_light_projection_bind_group =
            device.create_bind_group(&wgpu::BindGroupDescriptor {
                layout: &light_projection_bind_group_layout,
                entries: &[wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::Buffer(wgpu::BufferBinding {
                        buffer: &directional_light_projection_buffer,
                        offset: 0,
                        size: wgpu::BufferSize::new(matrix_uniform_offset),
                    }),
                }],
                label: Some("shadow_renderer_directional_light_projection_bind_group"),
            });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("shadow_renderer_render_pipeline_layout"),
                bind_group_layouts: &[
                    &model_matrix_bind_group_layout,
                    &light_projection_bind_group_layout,
                ],
                push_constant_ranges: &[],
            });
        let render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("shadow_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[vertex::layout()], // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: None,
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: None,                  //Some(wgpu::Face::Back),
                polygon_mode: wgpu::PolygonMode::Fill,
                unclipped_depth: false,
                conservative: false,
            },
            depth_stencil: Some(wgpu::DepthStencilState {
                format: wgpu::TextureFormat::Depth32Float,
                depth_write_enabled: true,
                depth_compare: wgpu::CompareFunction::Less,
                stencil: wgpu::StencilState::default(),
                bias: wgpu::DepthBiasState {
                    constant: 2,
                    slope_scale: 2.0,
                    clamp: 0.0,
                },
            }),
            multisample: wgpu::MultisampleState {
                count: 1,
                mask: !0,
                alpha_to_coverage_enabled: false,
            },
            multiview: None,
            cache: None,
        };

        let render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        let lightmap_sampler = device.create_sampler(&wgpu::SamplerDescriptor {
            // 4.
            address_mode_u: wgpu::AddressMode::ClampToEdge,
            address_mode_v: wgpu::AddressMode::ClampToEdge,
            address_mode_w: wgpu::AddressMode::ClampToEdge,
            mag_filter: wgpu::FilterMode::Linear,
            min_filter: wgpu::FilterMode::Linear,
            mipmap_filter: wgpu::FilterMode::Nearest,
            compare: Some(wgpu::CompareFunction::LessEqual), // 5.
            lod_min_clamp: 0.0,
            lod_max_clamp: 100.0,
            ..Default::default()
        });

        let directional_light_maps: [wgpu::TextureView;
            LightRenderer::MAX_DIRECTIONAL_LIGHTS as usize] =
            std::array::from_fn(|_| Self::create_lightmap_texture_view(device));
        Self {
            matrix_uniform_offset,
            lightmap_sampler,
            directional_light_maps: directional_light_maps,
            render_pipeline,
            directional_light_projection_buffer,
            queue: queue.clone(),
            device: device.clone(),
            queued_directional_light_shadow_passes: Vec::new(),
            num_directional_lights: 0,
            directional_light_projection_bind_group,
        }
    }

    // Call this function, then send all meshes that should be in the shadow pass to the render queue.
    pub fn start_queued_directional_light_shadow_pass<'a>(
        &'a mut self,
        encoder: &'a mut wgpu::CommandEncoder,
    ) -> Option<wgpu::RenderPass<'a>> {
        if self.queued_directional_light_shadow_passes.len() == 0 {
            return None;
        }
        let directional_light_idx: usize = self.queued_directional_light_shadow_passes
            [self.queued_directional_light_shadow_passes.len() - 1]
            as usize;

        let mut rp = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: Some("Render Pass"),
            color_attachments: &[],
            depth_stencil_attachment: Some(wgpu::RenderPassDepthStencilAttachment {
                view: &self.directional_light_maps[directional_light_idx],
                depth_ops: Some(wgpu::Operations {
                    load: wgpu::LoadOp::Clear(1.0),
                    store: wgpu::StoreOp::Store,
                }),
                stencil_ops: None,
            }),
            occlusion_query_set: None,
            timestamp_writes: None,
        });
        rp.set_pipeline(&self.render_pipeline);
        rp.set_bind_group(
            1,
            &self.directional_light_projection_bind_group,
            &[(self.matrix_uniform_offset as usize * directional_light_idx) as u32],
        );
        self.queued_directional_light_shadow_passes.pop();
        Some(rp)
    }

    fn create_lightmap_texture_view(device: &wgpu::Device) -> wgpu::TextureView {
        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: wgpu::TextureFormat::Depth32Float,
            width: 1024,
            height: 1024,
            present_mode: wgpu::PresentMode::Immediate,
            alpha_mode: wgpu::CompositeAlphaMode::Auto,
            view_formats: vec![],
            desired_maximum_frame_latency: 2,
        };
        let size = wgpu::Extent3d {
            // 2.
            width: config.width.max(1),
            height: config.height.max(1),
            depth_or_array_layers: 1,
        };
        let desc = wgpu::TextureDescriptor {
            label: Some("directional_light_map"),
            size,
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            format: wgpu::TextureFormat::Depth32Float,
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT // 3.
                | wgpu::TextureUsages::TEXTURE_BINDING,
            view_formats: &[],
        };
        device
            .create_texture(&desc)
            .create_view(&wgpu::TextureViewDescriptor::default())
    }

    pub fn reprocess_all(&mut self) {
        for n in 0..self.num_directional_lights {
            self.queued_directional_light_shadow_passes.push(n);
        }
    }

    pub fn get_directional_lightmaps(&self) -> [&wgpu::TextureView; 256] {
        self.directional_light_maps.each_ref()
    }
    pub fn get_lightmap_sampler(&self) -> &wgpu::Sampler {
        &self.lightmap_sampler
    }

    // pub fn add_directional_light_projection(&mut self, projection: &glam::Mat4) {
    //     self.queue.write_buffer(
    //         &self.directional_light_projection_buffer,
    //         self.num_directional_lights as u64 * self.matrix_uniform_offset,
    //         bytemuck::bytes_of(projection),
    //     );
    //     self.queued_directional_light_shadow_passes
    //         .push(self.num_directional_lights);
    //     self.num_directional_lights += 1;
    // }
    pub fn update_directional_light_projection(&mut self, id: u32, projection: &glam::Mat4) {
        self.queue.write_buffer(
            &self.directional_light_projection_buffer,
            id as u64 * self.matrix_uniform_offset,
            bytemuck::bytes_of(projection),
        );
        self.queued_directional_light_shadow_passes.push(id);
    }
}
