mod line;
use self::line::Line;

//render_pass.set_vertex_buffer(0, self.buffer.slice(..));
//render_pass.draw(0..6, 0..self.length);

pub struct LineRenderer {
    pub render_pipeline: wgpu::RenderPipeline,
    pub bind_group: wgpu::BindGroup,
    camera_buffer: wgpu::Buffer,
    camera_pos_buffer: wgpu::Buffer,

    lines: Vec<Line>,
}

const SHADER_PATH: &str = "res/shaders/line_shader.wgsl";

impl LineRenderer {
    pub fn layout() -> wgpu::VertexBufferLayout<'static> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<[f32; 12]>() as wgpu::BufferAddress, // 1.
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
            ],
        }
    }

    pub fn new(renderer: &Renderer, ln: &Vec<Point3<f32>>) -> LineRenderer {
        let bind_group_layout =
            renderer
                .device
                .create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
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

        let camera_buffer = renderer.device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Camera Buffer"),
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let camera_pos_buffer = renderer.device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Camera Position Buffer"),
            size: size_of::<[f32; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        LineRenderer {
            render_pipeline: renderer
                .create_default_render_pipeline(SHADER_PATH, &bind_group_layout, Self::layout())
                .expect("Failed to create universe simulation render pipeline!"),
            bind_group: renderer
                .device
                .create_bind_group(&wgpu::BindGroupDescriptor {
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
                }),
            camera_buffer: camera_buffer,
            camera_pos_buffer: camera_pos_buffer,
        }
    }

    pub fn upload_camera_matrix(&mut self, renderer: &Renderer, matrix: &[[f32; 4]; 4]) {
        renderer
            .queue
            .write_buffer(&self.camera_buffer, 0, bytemuck::cast_slice(matrix));
    }
    pub fn upload_camera_position(&mut self, renderer: &Renderer, position: &[f32; 3]) {
        renderer.queue.write_buffer(
            &self.camera_pos_buffer,
            0,
            bytemuck::bytes_of(&[position[0], position[1], position[2], 0.0]),
        );
    }
    pub fn render(&self, renderer: &mut Renderer, line: &[&dyn renderer::Renderable; 5]) {
        renderer
            .render(line, &self.render_pipeline, &self.bind_group)
            .unwrap();
    }
}
