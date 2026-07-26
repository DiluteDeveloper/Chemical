use crate::renderer::{Renderer, primary_textures};

#[derive(Debug)]
pub struct PostProcessRenderer {
    render_pipeline: wgpu::RenderPipeline,
    bind_group: wgpu::BindGroup,
    bind_group_layout: wgpu::BindGroupLayout,

    queue: wgpu::Queue,
    device: wgpu::Device,
}

impl PostProcessRenderer {
    const POST_PROCESS_SHADER_PATH: &str = "res/shaders/post_process_shader.wgsl";

    pub fn new(
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        framebuffer_view: &wgpu::TextureView,
        render_texture_format: wgpu::TextureFormat,
    ) -> Self {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    // Framebuffer sampler
                    binding: 0,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::NonFiltering),
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    // Framebuffer texture
                    binding: 1,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        sample_type: wgpu::TextureSampleType::Float { filterable: false },
                        view_dimension: wgpu::TextureViewDimension::D2,
                        multisampled: false,
                    },
                    count: None,
                },
            ],
            label: Some("post_process_renderer_bind_group_layout"),
        });
        let shader_source =
            std::fs::read_to_string(Self::POST_PROCESS_SHADER_PATH).expect(&format!(
                "Shader path '{}' was invalid!",
                Self::POST_PROCESS_SHADER_PATH
            ));

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(Self::POST_PROCESS_SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(shader_source.into()),
        });

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::Sampler(
                        &primary_textures::create_float_framebuffer_sampler(device),
                    ),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::TextureView(framebuffer_view),
                },
            ],
            label: Some("post_process_renderer_bind_group"),
        });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("shadow_renderer_render_pipeline_layout"),
                bind_group_layouts: &[&bind_group_layout],
                push_constant_ranges: &[],
            });
        let render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("post_process_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[],                 // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &shader,
                entry_point: Some("fs_main"),
                targets: &[Some(wgpu::ColorTargetState {
                    // 4.
                    format: render_texture_format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: Some(wgpu::Face::Front),
                polygon_mode: wgpu::PolygonMode::Fill,
                unclipped_depth: false,
                conservative: false,
            },
            depth_stencil: None,
            multisample: wgpu::MultisampleState {
                count: Renderer::MSAA_SAMPLE_COUNT,
                mask: !0,
                alpha_to_coverage_enabled: false,
            },
            multiview: None,
            cache: None,
        };

        let render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        Self {
            bind_group_layout,
            bind_group,
            device: device.clone(),
            queue: queue.clone(),
            render_pipeline,
        }
    }

    pub fn update_framebuffer_view(&mut self, framebuffer_view: &wgpu::TextureView) {
        self.bind_group = self.device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &self.bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::Sampler(
                        &primary_textures::create_float_framebuffer_sampler(&self.device),
                    ),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::TextureView(framebuffer_view),
                },
            ],
            label: Some("post_process_renderer_bind_group"),
        });
    }

    pub fn render(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_pipeline(&self.render_pipeline);
        render_pass.set_bind_group(0, &self.bind_group, &[]);
        render_pass.draw(0..3, 0..1);
    }
}

//    outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
//   gl_Position = vec4(outUV.x - ((outUV.x ^ 2) >> 1),(outUV.y - ((outUV.y ^ 2) >> 1)) * -1.0f, 0.0f, 1.0f);
// vertex 0 UV: 0, 0
// vertex 1 UV: 2, 0
// vertex 2 UV: 0, 2
// vertex 0 NDC: -1, 1
// vertex 1 NDC: 2, 1
// vertex 2 NDC: -1, -2
