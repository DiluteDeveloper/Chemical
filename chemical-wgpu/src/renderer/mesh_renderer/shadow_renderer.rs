use super::MeshScene;
use super::Vertex;
use crate::renderer::Texture;
use crate::renderer::mesh_renderer::Renderable;
pub struct ShadowRenderer {
    pub depth_texture: Texture,
    render_pipeline: wgpu::RenderPipeline,
    bind_group: wgpu::BindGroup,
}

const SHADOW_SHADER_PATH: &str = "res/shaders/shadow_shader.wgsl";

impl ShadowRenderer {
    pub fn new(
        light_buffer: &wgpu::Buffer,
        model_buffer: &wgpu::Buffer,
        device: &wgpu::Device,
        surface_caps: &wgpu::SurfaceCapabilities,
        aligned_model_matrix_size_offset: u64,
    ) -> Self {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 1,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: true,
                        min_binding_size: None,
                    },
                    count: None,
                },
            ],
            label: Some("mesh_renderer_bind_group_layout"),
        });
        let shader_source = std::fs::read_to_string(SHADOW_SHADER_PATH).expect(&format!(
            "Shader path '{}' was invalid!",
            SHADOW_SHADER_PATH
        ));

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(SHADOW_SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(shader_source.into()),
        });

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: light_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Buffer(wgpu::BufferBinding {
                        buffer: &model_buffer,
                        offset: 0,
                        size: wgpu::BufferSize::new(aligned_model_matrix_size_offset),
                    }),
                },
            ],
            label: Some("shadow_renderer_bind_group"),
        });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("shadow_renderer_render_pipeline_layout"),
                bind_group_layouts: &[&bind_group_layout],
                immediate_size: 0,
            });
        let render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("shadow_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[Vertex::layout()], // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: None,
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: Some(wgpu::Face::Back),
                // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                polygon_mode: wgpu::PolygonMode::Fill,
                // Requires Features::DEPTH_CLIP_CONTROL
                unclipped_depth: false,
                // Requires Features::CONSERVATIVE_RASTERIZATION
                conservative: false,
            },
            depth_stencil: Some(wgpu::DepthStencilState {
                format: Texture::DEPTH_FORMAT,
                depth_write_enabled: true,
                depth_compare: wgpu::CompareFunction::Less,
                stencil: wgpu::StencilState::default(),
                bias: wgpu::DepthBiasState {
                    constant: 2,
                    slope_scale: 2.0,
                    clamp: 0.0,
                },
            }), // 1.
            multisample: wgpu::MultisampleState {
                count: 1,                         // 2.
                mask: !0,                         // 3.
                alpha_to_coverage_enabled: false, // 4.
            },
            multiview_mask: None, // 5.
            cache: None,          // 6.
        };

        let render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: wgpu::TextureFormat::Depth32Float,
            width: 1024,
            height: 1024,
            present_mode: surface_caps.present_modes[0],
            alpha_mode: surface_caps.alpha_modes[0],
            view_formats: vec![],
            desired_maximum_frame_latency: 2,
        };
        let depth_texture = Texture::create_depth_texture(
            &device,
            config.width,
            config.height,
            "shadow_renderer_depth_texture",
        );
        Self {
            depth_texture: depth_texture,
            render_pipeline: render_pipeline,
            bind_group: bind_group,
        }
    }
    pub fn build_shadow_pass(&self, mesh_scene: &MeshScene, encoder: &mut wgpu::CommandEncoder) {
        let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
            label: Some("Render Pass"),
            color_attachments: &[],
            depth_stencil_attachment: Some(wgpu::RenderPassDepthStencilAttachment {
                view: &self.depth_texture.view,
                depth_ops: Some(wgpu::Operations {
                    load: wgpu::LoadOp::Clear(1.0),
                    store: wgpu::StoreOp::Store,
                }),
                stencil_ops: None,
            }),
            occlusion_query_set: None,
            timestamp_writes: None,
            multiview_mask: None,
        });
        render_pass.set_pipeline(&self.render_pipeline);

        for (mesh, transform_id) in mesh_scene.lit_vertex_meshes.iter() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(*transform_id as u64 * mesh_scene.aligned_model_matrix_size_offset) as u32],
            );
            mesh.bind(&mut render_pass);
            mesh.draw(&mut render_pass);
        }
        for (mesh, transform_id) in mesh_scene.lit_index_meshes.iter() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(*transform_id as u64 * mesh_scene.aligned_model_matrix_size_offset) as u32],
            );
            mesh.bind(&mut render_pass);
            mesh.draw(&mut render_pass);
        }
    }
}
