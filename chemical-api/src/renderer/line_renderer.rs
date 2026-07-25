mod line;

use line::LineSegment;
pub use line::{Line, LineDescriptor};

use super::Texture;
use crate::{Camera, renderer::Renderer};

#[derive(Debug)]
pub struct LineRenderer {
    pub render_pipeline: wgpu::RenderPipeline,
    pub bind_group: wgpu::BindGroup,
    camera_buffer: wgpu::Buffer,
    camera_pos_buffer: wgpu::Buffer,
    line_buffer: wgpu::Buffer,

    lines: Vec<Line>,
}

const SHADER_PATH: &str = "res/shaders/line_shader.wgsl";
const OVERALL_MAX_LINE_SEGMENT_COUNT: u64 = 100000;

type LineID = usize;

impl LineRenderer {
    fn layout() -> wgpu::VertexBufferLayout<'static> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<[f32; 17]>() as wgpu::BufferAddress, // 1.
            step_mode: wgpu::VertexStepMode::Instance,                             // 2.
            attributes: &[
                // 3.
                wgpu::VertexAttribute {
                    offset: 0,                             // 4.
                    shader_location: 0,                    // 5.
                    format: wgpu::VertexFormat::Float32x3, // 6.
                },
                wgpu::VertexAttribute {
                    offset: std::mem::size_of::<[f32; 3]>() as wgpu::BufferAddress,
                    shader_location: 1,
                    format: wgpu::VertexFormat::Float32x3,
                },
                wgpu::VertexAttribute {
                    offset: std::mem::size_of::<[f32; 6]>() as wgpu::BufferAddress,
                    shader_location: 2,
                    format: wgpu::VertexFormat::Float32x3,
                },
                wgpu::VertexAttribute {
                    offset: std::mem::size_of::<[f32; 9]>() as wgpu::BufferAddress,
                    shader_location: 3,
                    format: wgpu::VertexFormat::Float32x3,
                },
                wgpu::VertexAttribute {
                    offset: std::mem::size_of::<[f32; 12]>() as wgpu::BufferAddress,
                    shader_location: 4,
                    format: wgpu::VertexFormat::Float32x4,
                },
                wgpu::VertexAttribute {
                    offset: std::mem::size_of::<[f32; 16]>() as wgpu::BufferAddress,
                    shader_location: 5,
                    format: wgpu::VertexFormat::Float32,
                },
            ],
        }
    }

    pub(super) fn new(device: &wgpu::Device, texture_format: wgpu::TextureFormat) -> Self {
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
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
            ],
            label: Some("line_renderer_bind_group_layout"),
        });

        let camera_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("camera_buffer"),
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let camera_pos_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("camera_pos_buffer"),
            size: size_of::<[f32; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let line_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("line_buffer"),
            size: size_of::<LineSegment>() as u64 * OVERALL_MAX_LINE_SEGMENT_COUNT,
            usage: wgpu::BufferUsages::VERTEX | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let shader_source = std::fs::read_to_string(SHADER_PATH)
            .expect(&format!("Shader path '{}' was invalid!", SHADER_PATH));

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(shader_source.into()),
        });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("line_renderer_render_pipeline_layout"),
                bind_group_layouts: &[&bind_group_layout],
                push_constant_ranges: &[],
            });
        let render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("line_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[Self::layout()],   // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &shader,
                entry_point: Some("fs_main"),
                targets: &[Some(wgpu::ColorTargetState {
                    // 4.
                    format: texture_format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: None,
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
                bias: wgpu::DepthBiasState::default(),
            }), // 1.
            multisample: wgpu::MultisampleState {
                count: Renderer::MSAA_SAMPLE_COUNT, // 2.
                mask: !0,                           // 3.
                alpha_to_coverage_enabled: false,   // 4.
            },
            multiview: None,
            cache: None, // 6.
        };
        let render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: camera_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: camera_pos_buffer.as_entire_binding(),
                },
            ],
            label: Some("line_renderer_bind_group"),
        });
        LineRenderer {
            render_pipeline: render_pipeline,
            bind_group: bind_group,
            camera_buffer: camera_buffer,
            camera_pos_buffer: camera_pos_buffer,
            line_buffer: line_buffer,
            lines: Vec::new(),
        }
    }
    pub fn create_line(&mut self, descriptor: &LineDescriptor) -> LineID {
        self.lines.push(Line::new(descriptor));
        self.lines.len() - 1
    }
    pub fn get_line(&mut self, line_id: LineID) -> Option<&mut Line> {
        self.lines.get_mut(line_id)
    }

    pub(super) fn render(
        &mut self,
        camera: &Camera,
        queue: &wgpu::Queue,
        render_pass: &mut wgpu::RenderPass,
    ) {
        render_pass.set_pipeline(&self.render_pipeline);

        render_pass.set_bind_group(0, &self.bind_group, &[]);

        queue.write_buffer(
            &self.camera_buffer,
            0,
            bytemuck::bytes_of(&camera.get_camera_matrix()),
        );
        let pos = camera.transform.position;
        queue.write_buffer(
            &self.camera_pos_buffer,
            0,
            bytemuck::bytes_of(&[pos[0], pos[1], pos[2], 0.0]),
        );

        let total_segments = self.fill_line_buffer(queue);

        render_pass.set_vertex_buffer(0, self.line_buffer.slice(..));
        render_pass.draw(0..6, 0..total_segments as u32);
    }

    fn fill_line_buffer(&mut self, queue: &wgpu::Queue) -> u64 {
        let mut total_segments: u64 = 0;
        for line in self.lines.iter() {
            if line.segments.len() == 0 {
                continue;
            }
            queue.write_buffer(
                &self.line_buffer,
                total_segments * (size_of::<LineSegment>() as u64),
                bytemuck::cast_slice(&line.segments),
            );
            total_segments += line.segments.len() as u64;
        }
        total_segments
    }
}
