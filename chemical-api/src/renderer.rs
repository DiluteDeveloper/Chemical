pub mod line_renderer;
pub mod mesh_renderer;
mod texture;

use crate::scene::SceneContainer;
pub use line_renderer::LineRenderer;
pub use mesh_renderer::MeshRenderer;
pub use texture::Texture;

use log::info;
use std::sync::Arc;
use winit::{dpi::PhysicalSize, window::Window};

use crate::Camera;

#[derive(Debug, Clone, PartialEq)]
pub enum RenderTarget {
    Window(PhysicalSize<u32>),
    Custom(PhysicalSize<u32>, wgpu::Origin3d),
}

#[derive(Debug)]
pub struct RendererDescriptor {
    pub texture_format: wgpu::TextureFormat,
    pub device: wgpu::Device,
    pub queue: wgpu::Queue,
    pub surface: wgpu::Surface<'static>,
    pub render_target: RenderTarget,
    pub surface_config: wgpu::SurfaceConfiguration,
    // pub window: Arc<Window>,
}
#[derive(Debug)]
pub struct Renderer {
    depth_texture: Texture,
    msaa_view: wgpu::TextureView,
    texture_format: wgpu::TextureFormat,
    surface: wgpu::Surface<'static>,
    custom_render_target: Option<wgpu::Texture>,
    render_target: RenderTarget,
    surface_config: wgpu::SurfaceConfiguration,

    device: wgpu::Device,
    queue: wgpu::Queue,

    mesh_renderer: MeshRenderer,
    pub line_renderer: LineRenderer,
}

impl Renderer {
    const MSAA_SAMPLE_COUNT: u32 = 8;

    pub fn new(descriptor: RendererDescriptor) -> Self {
        // let (surface, texture_format, present_mode, alpha_mode, device, queue, adapter) =
        //     Self::init(window).await?;

        match descriptor.render_target {
            RenderTarget::Custom(size, _) => {
                // let window_size = descriptor.window.inner_size();
                let depth_texture = Texture::create_depth_texture(
                    &descriptor.device,
                    size.width,
                    size.height,
                    "Depth Texture",
                );
                let msaa_view = Self::create_msaa_view(
                    &descriptor.device,
                    descriptor.texture_format,
                    size.width,
                    size.height,
                );
                let custom_render_target = Some(Self::create_custom_target(
                    &descriptor.device,
                    descriptor.texture_format,
                    size.width,
                    size.height,
                ));
                Self {
                    mesh_renderer: MeshRenderer::new(
                        &descriptor.device,
                        descriptor.texture_format,
                        &descriptor.queue,
                    ),
                    line_renderer: LineRenderer::new(&descriptor.device, descriptor.texture_format),
                    depth_texture,
                    msaa_view,
                    texture_format: descriptor.texture_format,
                    device: descriptor.device,
                    queue: descriptor.queue,
                    surface: descriptor.surface,
                    surface_config: descriptor.surface_config,
                    custom_render_target,
                    render_target: descriptor.render_target,
                }
            }
            RenderTarget::Window(size) => {
                let depth_texture = Texture::create_depth_texture(
                    &descriptor.device,
                    size.width,
                    size.height,
                    "Depth Texture",
                );
                let msaa_view = Self::create_msaa_view(
                    &descriptor.device,
                    descriptor.texture_format,
                    size.width,
                    size.height,
                );
                Self {
                    mesh_renderer: MeshRenderer::new(
                        &descriptor.device,
                        descriptor.texture_format,
                        &descriptor.queue,
                    ),
                    line_renderer: LineRenderer::new(&descriptor.device, descriptor.texture_format),
                    depth_texture,
                    msaa_view,
                    texture_format: descriptor.texture_format,
                    device: descriptor.device,
                    queue: descriptor.queue,
                    surface: descriptor.surface,
                    custom_render_target: None,
                    render_target: descriptor.render_target,
                    surface_config: descriptor.surface_config,
                }
            }
        }
    }

    pub async fn initialize_window_surface(
        window: Arc<Window>,
        size: PhysicalSize<u32>,
        is_render_target: bool,
    ) -> anyhow::Result<(
        wgpu::Surface<'static>,
        wgpu::TextureFormat,
        // wgpu::PresentMode,
        // wgpu::CompositeAlphaMode,
        wgpu::Device,
        wgpu::Queue,
        wgpu::Adapter,
        wgpu::SurfaceConfiguration,
    )> {
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
        // let mut present_mode_idx = 0;

        // for (i, present_mode) in surface_caps.present_modes.iter().enumerate() {
        //     if (*present_mode) == wgpu::PresentMode::Immediate {
        //         present_mode_idx = i;
        //     }
        // }

        let surface_format = surface_caps
            .formats
            .iter()
            .find(|f| f.is_srgb())
            .copied()
            .unwrap_or(surface_caps.formats[0]);

        let usage = match is_render_target {
            false => wgpu::TextureUsages::RENDER_ATTACHMENT | wgpu::TextureUsages::COPY_DST,
            true => wgpu::TextureUsages::RENDER_ATTACHMENT,
        };
        let surface_config = wgpu::SurfaceConfiguration {
            usage: usage,
            format: surface_format,
            width: size.width,
            height: size.height,
            present_mode: surface_caps.present_modes[0],
            alpha_mode: surface_caps.alpha_modes[0],
            view_formats: vec![],
            desired_maximum_frame_latency: 2,
        };
        surface.configure(&device, &surface_config);

        Ok((
            surface,
            surface_format,
            // surface_caps.present_modes[0],
            // surface_caps.alpha_modes[0],
            device,
            queue,
            adapter,
            surface_config,
        ))
    }

    pub fn resize(&mut self, render_target: RenderTarget) {
        match render_target {
            RenderTarget::Window(size) => {
                self.surface_config.width = size.width;
                self.surface_config.height = size.height;
                self.surface.configure(&self.device, &self.surface_config);
            }
            RenderTarget::Custom(size, _) => {
                self.depth_texture = Texture::create_depth_texture(
                    &self.device,
                    size.width,
                    size.height,
                    "Depth Texture",
                );
                self.msaa_view = Self::create_msaa_view(
                    &self.device,
                    self.texture_format,
                    size.width,
                    size.height,
                );
                self.custom_render_target = Some(Self::create_custom_target(
                    &self.device,
                    self.texture_format,
                    size.width,
                    size.height,
                ));
                self.render_target = render_target;
            }
        }
    }
    fn create_msaa_view(
        device: &wgpu::Device,
        format: wgpu::TextureFormat,
        width: u32,
        height: u32,
    ) -> wgpu::TextureView {
        device
            .create_texture(&wgpu::TextureDescriptor {
                label: Some("MSAA texture"),
                size: wgpu::Extent3d {
                    width: width,
                    height: height,
                    depth_or_array_layers: 1,
                },
                mip_level_count: 1,
                sample_count: Self::MSAA_SAMPLE_COUNT,
                dimension: wgpu::TextureDimension::D2,
                format: format,
                usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
                view_formats: &[],
            })
            .create_view(&wgpu::TextureViewDescriptor::default())
    }
    fn create_custom_target(
        device: &wgpu::Device,
        format: wgpu::TextureFormat,
        width: u32,
        height: u32,
    ) -> wgpu::Texture {
        device.create_texture(&wgpu::TextureDescriptor {
            label: Some("Custom target texture"),
            size: wgpu::Extent3d {
                width: width,
                height: height,
                depth_or_array_layers: 1,
            },
            mip_level_count: 1,
            sample_count: 1,
            dimension: wgpu::TextureDimension::D2,
            format: format,
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT | wgpu::TextureUsages::COPY_SRC,
            view_formats: &[],
        })
    }
    pub fn prepare(&mut self, camera: &Camera) -> wgpu::CommandEncoder {
        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("Render Encoder"),
            });
        self.mesh_renderer.prepare(&camera, &mut encoder);
        encoder
    }

    pub fn render(
        &mut self,
        camera: &Camera,
        encoder: &mut wgpu::CommandEncoder,
    ) -> anyhow::Result<wgpu::SurfaceTexture, wgpu::SurfaceError> {
        // let output = self.surface.get_current_texture()?;
        // let view = output
        //     .texture
        //     .create_view(&wgpu::TextureViewDescriptor::default());
        let surface_texture = self.surface.get_current_texture()?;
        let surface_texture_tex = &surface_texture.texture;

        let render_target_texture = match &self.custom_render_target {
            Some(vw) => vw,
            None => &surface_texture_tex,
        };
        let view = render_target_texture.create_view(&wgpu::TextureViewDescriptor::default());

        {
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
        Ok(surface_texture)
    }
    pub fn paste_render_texture(
        &self,
        encoder: &mut wgpu::CommandEncoder,
        surface_texture: &wgpu::SurfaceTexture,
    ) {
        match self.render_target {
            RenderTarget::Custom(size, offset) => {
                encoder.copy_texture_to_texture(
                    self.custom_render_target
                        .as_ref()
                        .expect("Custom render target is None while render_target is Custom!")
                        .as_image_copy(),
                    wgpu::TexelCopyTextureInfo {
                        texture: &surface_texture.texture,
                        mip_level: 0,
                        origin: wgpu::Origin3d {
                            x: offset.x,
                            y: offset.y,
                            z: 0,
                        },
                        aspect: wgpu::TextureAspect::All,
                    },
                    wgpu::Extent3d {
                        width: size.width,
                        height: size.height,
                        depth_or_array_layers: 1,
                    },
                );
            }
            _ => (),
        }
    }
    pub fn submit(&self, encoder: wgpu::CommandEncoder) {
        self.queue.submit(std::iter::once(encoder.finish()));
    }
    pub fn present(&self, surface_texture: wgpu::SurfaceTexture) {
        surface_texture.present();
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
