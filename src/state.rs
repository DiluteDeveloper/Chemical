use log::info;
use std::sync::Arc;
use winit::{event_loop::ActiveEventLoop, keyboard::KeyCode, window::Window};

use crate::utility::fps_counter::FPSCounter;

use crate::camera;
use crate::geometry::{sphere, vertex};
use crate::rendering::mesh::{self, Renderable};
use wgpu::util::DeviceExt;

pub struct State {
    pub surface: wgpu::Surface<'static>,
    device: wgpu::Device,
    queue: wgpu::Queue,
    config: wgpu::SurfaceConfiguration,
    is_surface_configured: bool,
    render_pipeline: [wgpu::RenderPipeline; 2],
    is_alternate_pipeline_active: bool,
    window: Arc<Window>,
    mesh: mesh::IndexMesh,
    camera: camera::Camera,
    camera_buffer: wgpu::Buffer,
    camera_bind_group: wgpu::BindGroup,
    pub camera_controller: camera::CameraController,
    camera_uniform: [[f32; 4]; 4],
    fps_counter: FPSCounter,
}

impl State {
    pub async fn new(window: Arc<Window>) -> anyhow::Result<State> {
        let size = window.inner_size();

        // The instance is a handle to our GPU
        // BackendBit::PRIMARY => Vulkan + Metal + DX12 + Browser WebGPU
        let instance = wgpu::Instance::new(&wgpu::InstanceDescriptor {
            backends: wgpu::Backends::PRIMARY,
            ..Default::default()
        });

        let surface = instance.create_surface(window.clone()).unwrap();

        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                compatible_surface: Some(&surface),
                force_fallback_adapter: false,
            })
            .await?;

        let info = adapter.get_info();
        info!(
            "GPU: {} ({:?}), backend: {:?}",
            info.name, info.device_type, info.backend
        );

        let (device, queue) = adapter
            .request_device(&wgpu::DeviceDescriptor {
                label: None,
                required_features: wgpu::Features::empty(),
                experimental_features: wgpu::ExperimentalFeatures::disabled(),
                required_limits: wgpu::Limits::default(),
                memory_hints: Default::default(),
                trace: wgpu::Trace::Off,
            })
            .await?;

        let surface_caps = surface.get_capabilities(&adapter);
        let mut present_mode_idx = 0;

        for (i, present_mode) in surface_caps.present_modes.iter().enumerate() {
            if (*present_mode) == wgpu::PresentMode::AutoVsync {
                present_mode_idx = i;
            }
        }
        // Shader code in this tutorial assumes an sRGB surface texture. Using a different
        // one will result in all the colors coming out darker. If you want to support non
        // sRGB surfaces, you'll need to account for that when drawing to the frame.
        let surface_format = surface_caps
            .formats
            .iter()
            .find(|f| f.is_srgb())
            .copied()
            .unwrap_or(surface_caps.formats[0]);
        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface_format,
            width: size.width,
            height: size.height,
            present_mode: surface_caps.present_modes[present_mode_idx],
            alpha_mode: surface_caps.alpha_modes[0],
            view_formats: vec![],
            desired_maximum_frame_latency: 2,
        };
        let shader = device.create_shader_module(wgpu::include_wgsl!("shader.wgsl"));
        let shader2 = device.create_shader_module(wgpu::include_wgsl!("shader.wgsl"));

        let mut camera =
            camera::Camera::new(config.width as f32 / config.height as f32, 45.0, 0.1, 100.0);
        camera.position = (0.0, 0.0, 10.0).into();
        let camera_uniform = camera.to_matrix().unwrap().into();

        let camera_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Camera Buffer"),
            contents: bytemuck::cast_slice(&[camera_uniform]),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
        });
        let camera_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                }],
                label: Some("camera_bind_group_layout"),
            });
        let camera_bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &camera_bind_group_layout,
            entries: &[wgpu::BindGroupEntry {
                binding: 0,
                resource: camera_buffer.as_entire_binding(),
            }],
            label: Some("camera_bind_group"),
        });
        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("Render Pipeline Layout"),
                bind_group_layouts: &[&camera_bind_group_layout],
                immediate_size: 0,
            });
        //let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
        let mut render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("Render Pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"),         // 1.
                buffers: &[vertex::Vertex::layout()], // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &shader,
                entry_point: Some("fs_main"),
                targets: &[Some(wgpu::ColorTargetState {
                    // 4.
                    format: config.format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            }),
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
            depth_stencil: None, // 1.
            multisample: wgpu::MultisampleState {
                count: 1,                         // 2.
                mask: !0,                         // 3.
                alpha_to_coverage_enabled: false, // 4.
            },
            multiview_mask: None, // 5.
            cache: None,          // 6.
        };
        let shader_render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        render_pipeline_descriptor.vertex.module = &shader2;
        match render_pipeline_descriptor.fragment.as_mut() {
            Some(fragment) => fragment.module = &shader2,
            None => panic!("No fragment shader!"),
        }
        let shader2_render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);

        let camera_controller = camera::CameraController::new(0.02, 0.002, 0.2);

        let (vertices, indices) = sphere::generate_index_sphere(255)
            .map_err(|e| anyhow::anyhow!("Failed to generate vertex sphere: {}", e))?;

        let mesh = mesh::IndexMesh::new(&vertices, &indices, &device);

        Ok(Self {
            mesh,
            surface,
            device,
            queue,
            config,
            is_surface_configured: false,
            is_alternate_pipeline_active: false,
            render_pipeline: [shader_render_pipeline, shader2_render_pipeline],
            window,
            camera,
            camera_buffer,
            camera_bind_group,
            camera_controller,
            camera_uniform,
            fps_counter: FPSCounter::new(),
        })
    }

    pub fn resize(&mut self, width: u32, height: u32) {
        if width > 0 && height > 0 {
            self.config.width = width;
            self.config.height = height;
            self.surface.configure(&self.device, &self.config);
            self.is_surface_configured = true;
        }
    }

    pub fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
        self.window.request_redraw();

        if !self.is_surface_configured {
            return Ok(());
        }

        let output = self.surface.get_current_texture()?;
        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });
        {
            let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &view,
                    resolve_target: None,
                    depth_slice: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color {
                            r: 0.1,
                            g: 0.2,
                            b: 0.3,
                            a: 1.0,
                        }),
                        store: wgpu::StoreOp::Store,
                    },
                })],
                depth_stencil_attachment: None,
                occlusion_query_set: None,
                timestamp_writes: None,
                multiview_mask: None,
            });
            if self.is_alternate_pipeline_active {
                render_pass.set_pipeline(&self.render_pipeline[1]);
            } else {
                render_pass.set_pipeline(&self.render_pipeline[0]);
            }
            //render_pass.set_bind_group(0, &self.diffuse_bind_group, &[]);
            // index must match the order of the bind group in the render pipeline descriptor
            render_pass.set_bind_group(0, &self.camera_bind_group, &[]);
            self.mesh.render(&mut render_pass);
        }

        // submit will accept anything that implements IntoIter
        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();

        Ok(())
    }
    pub fn update(&mut self) {
        self.camera_controller.update_camera(&mut self.camera);
        self.camera_uniform = self.camera.to_matrix().unwrap().into();
        self.queue.write_buffer(
            &self.camera_buffer,
            0,
            bytemuck::cast_slice(&[self.camera_uniform]),
        );
        self.fps_counter.update();
    }
    pub fn handle_key(&mut self, event_loop: &ActiveEventLoop, code: KeyCode, is_pressed: bool) {
        match (code, is_pressed) {
            (KeyCode::Escape, true) => event_loop.exit(),
            _ => {}
        }
        if self.camera_controller.is_enabled {
            self.camera_controller.handle_key(code, is_pressed);
        }
    }
    pub fn handle_mouse_moved(&mut self, _event_loop: &ActiveEventLoop, pos: &(f64, f64)) {
        if self.camera_controller.is_enabled {
            self.camera_controller.handle_mouse_moved(pos.into());
        }
    }
}
