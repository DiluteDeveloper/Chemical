pub mod line_renderer;
pub mod mesh_renderer;
mod texture;

use chemical_engine::scene::SceneContainer;
pub use line_renderer::LineRenderer;
pub use mesh_renderer::MeshRenderer;
pub use texture::Texture;

use log::info;
use std::sync::Arc;
use winit::window::Window;

use crate::Camera;

pub struct Renderer {
    surface: wgpu::Surface<'static>,
    is_surface_configured: bool,
    window: Arc<Window>,

    depth_texture: Texture,

    device: wgpu::Device,
    queue: wgpu::Queue,
    config: wgpu::SurfaceConfiguration,
    adapter: wgpu::Adapter,
    surface_format: wgpu::TextureFormat,

    mesh_renderer: MeshRenderer,
    pub line_renderer: LineRenderer,

    msaa_view: wgpu::TextureView,
}

impl Renderer {
    const MSAA_SAMPLE_COUNT: u32 = 8;
    pub async fn new(window: Arc<Window>) -> anyhow::Result<Renderer> {
        let size = window.inner_size();

        // The instance is a handle to our GPU
        // BackendBit::PRIMARY => Vulkan + Metal + DX12 + Browser WebGPU
        let instance = wgpu::Instance::new(&wgpu::InstanceDescriptor {
            backends: wgpu::Backends::PRIMARY,
            ..Default::default()
        });

        let surface = instance.create_surface(window.clone())?;

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

        let mut limits = wgpu::Limits::default();
        limits.max_binding_array_elements_per_shader_stage = 256;

        let (device, queue) = adapter
            .request_device(&wgpu::DeviceDescriptor {
                label: None,
                required_features: wgpu::Features {
                    features_wgpu: wgpu::FeaturesWGPU::TEXTURE_BINDING_ARRAY |
                    wgpu::FeaturesWGPU::SAMPLED_TEXTURE_AND_STORAGE_BUFFER_ARRAY_NON_UNIFORM_INDEXING |
                    wgpu::FeaturesWGPU::TEXTURE_ADAPTER_SPECIFIC_FORMAT_FEATURES,
                    features_webgpu: wgpu::FeaturesWebGPU::default(),
                },
                experimental_features: wgpu::ExperimentalFeatures::disabled(),
                required_limits: limits,
                memory_hints: Default::default(),
                trace: wgpu::Trace::Off,
            })
            .await?;

        let surface_caps = surface.get_capabilities(&adapter);
        let mut present_mode_idx = 0;

        for (i, present_mode) in surface_caps.present_modes.iter().enumerate() {
            if (*present_mode) == wgpu::PresentMode::Immediate {
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
        let depth_texture =
            Texture::create_depth_texture(&device, config.width, config.height, "Depth Texture");

        let msaa_view = Self::configure_msaa(&device, &config);

        Ok(Self {
            msaa_view,
            mesh_renderer: MeshRenderer::new(&device, &config, &queue),
            line_renderer: LineRenderer::new(&device, &config),
            surface,
            device,
            queue,
            config,
            is_surface_configured: false,
            window,
            depth_texture,
            adapter,
            surface_format,
        })
    }

    pub fn resize(&mut self, width: u32, height: u32) {
        if width > 0 && height > 0 {
            self.config.width = width;
            self.config.height = height;
            self.surface.configure(&self.device, &self.config);
            self.is_surface_configured = true;
            self.depth_texture = Texture::create_depth_texture(
                &self.device,
                self.config.width,
                self.config.height,
                "Depth Texture",
            );
            let msaa_view = Self::configure_msaa(&self.device, &self.config);
            self.msaa_view = msaa_view;
        }
    }
    fn configure_msaa(
        device: &wgpu::Device,
        config: &wgpu::SurfaceConfiguration,
    ) -> wgpu::TextureView {
        let msaa_texture = device.create_texture(&wgpu::TextureDescriptor {
            label: Some("MSAA texture"),
            size: wgpu::Extent3d {
                width: config.width,
                height: config.height,
                depth_or_array_layers: 1,
            },
            mip_level_count: 1,
            sample_count: Self::MSAA_SAMPLE_COUNT,
            dimension: wgpu::TextureDimension::D2,
            format: config.format, // match your surface/swapchain format
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            view_formats: &[],
        });
        msaa_texture.create_view(&wgpu::TextureViewDescriptor::default())
    }

    #[cfg(feature = "chemical-gui")]
    pub fn create_gui(&self) -> chemical_gui::ChemicalGUI {
        chemical_gui::ChemicalGUI::new(
            &self.device,
            &self.queue,
            &self.adapter,
            &self.surface_format,
            &self.window,
        )
    }

    pub fn render(&mut self, camera: &Camera) -> Result<(), wgpu::SurfaceError> {
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
        self.mesh_renderer.prepare(&camera, &mut encoder);

        // render lights buffer after encoder before real render pass
        {
            // Needs to be for the entire render pass all shaders
            let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &self.msaa_view,
                    resolve_target: Some(&view),
                    depth_slice: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color {
                            r: 0.01,
                            g: 0.01,
                            b: 0.01,
                            a: 1.0,
                        }),
                        store: wgpu::StoreOp::Store,
                    },
                })],
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
            });

            self.mesh_renderer.render(&mut render_pass);
            self.line_renderer
                .render(&camera, &self.queue, &mut render_pass);
        }

        self.queue.submit(std::iter::once(encoder.finish()));

        #[cfg(feature = "chemical-gui")]
        gui.redraw(&output);

        output.present();

        Ok(())
    }
    pub fn render_to_view(
        &mut self,
        encoder: &mut wgpu::CommandEncoder,
        view: &wgpu::TextureView,
        camera: &Camera,
    ) {
        self.mesh_renderer.prepare(&camera, encoder);

        // render lights buffer after encoder before real render pass
        {
            // Needs to be for the entire render pass all shaders
            let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &self.msaa_view,
                    resolve_target: Some(&view),
                    depth_slice: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color {
                            r: 0.01,
                            g: 0.01,
                            b: 0.01,
                            a: 1.0,
                        }),
                        store: wgpu::StoreOp::Store,
                    },
                })],
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
            });

            self.mesh_renderer.render(&mut render_pass);
            self.line_renderer
                .render(&camera, &self.queue, &mut render_pass);
        }
    }

    pub fn process_scene_operations(&mut self, scene: &mut SceneContainer) {
        scene
            .transform_handler
            .dispatch_operations(&mut self.mesh_renderer);
        scene
            .mesh_handler
            .dispatch_operations(&mut self.mesh_renderer);
        scene
            .light_handler
            .dispatch_operations(&mut self.mesh_renderer);
    }
}
