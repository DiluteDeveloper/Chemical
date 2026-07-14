use std::sync::Arc;
use std::time::Instant;

use crate::test::{self, Counter, Message};
use iced_wgpu::wgpu;
use iced_winit::runtime::user_interface;
use iced_winit::{core, winit};

pub struct ChemicalGUI {
    counter: test::Counter,
    viewport: iced_wgpu::graphics::Viewport,
    cache: user_interface::Cache,
    renderer: iced_wgpu::Renderer,
    window: Arc<winit::window::Window>,
    cursor: core::mouse::Cursor,
}

impl ChemicalGUI {
    pub fn new(
        device: &wgpu::Device,
        queue: &wgpu::Queue,
        adapter: &wgpu::Adapter,
        format: &wgpu::TextureFormat,
        window: Arc<winit::window::Window>,
    ) -> Self {
        let size = window.inner_size();
        let viewport = iced_wgpu::graphics::Viewport::with_physical_size(
            core::Size::new(size.width, size.height),
            window.scale_factor() as f32,
        );

        let renderer = {
            let engine = iced_wgpu::Engine::new(
                &adapter,
                device.clone(),
                queue.clone(),
                format.clone(),
                None,
                iced_wgpu::graphics::Shell::headless(),
            );

            iced_wgpu::Renderer::new(
                engine,
                iced_winit::core::Font::default(),
                iced_winit::core::Pixels(16.0),
            )
        };

        Self {
            counter: Counter::new(),
            viewport,
            cache: user_interface::Cache::new(),
            renderer,
            window: window,
            cursor: core::mouse::Cursor::Unavailable,
        }
    }
    pub fn resize(&mut self, size: &iced_winit::winit::dpi::PhysicalSize<u32>) {
        self.viewport = iced_wgpu::graphics::Viewport::with_physical_size(
            core::Size::new(size.width, size.height),
            self.window.scale_factor() as f32,
        );
    }

    pub fn redraw(&mut self, surface_texture: &wgpu::SurfaceTexture) {
        let view = surface_texture
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());

        let mut interface = user_interface::UserInterface::build(
            self.counter.view(),
            self.viewport.logical_size(),
            std::mem::take(&mut self.cache),
            &mut self.renderer,
        );

        //let counter_view = self.counter.view();

        let mut messages = Vec::new();
        let (state, _) = interface.update(
            &[core::Event::Window(core::window::Event::RedrawRequested(
                Instant::now(),
            ))],
            self.cursor,
            &mut self.renderer,
            &mut core::clipboard::Null,
            &mut messages,
        );

        if let user_interface::State::Updated {
            mouse_interaction, ..
        } = state
        {
            if let Some(icon) = iced_winit::conversion::mouse_interaction(mouse_interaction) {
                self.window.set_cursor(icon);
                self.window.set_cursor_visible(true);
            } else {
                self.window.set_cursor_visible(false);
            }
        }

        interface.draw(
            &mut self.renderer,
            &core::Theme::GruvboxLight,
            &core::renderer::Style::default(),
            self.cursor,
        );
        self.cache = interface.into_cache();

        let _ = self.renderer.present(
            None,
            surface_texture.texture.format(),
            &view,
            &self.viewport,
        );
    }

    pub fn cursor_moved(&mut self, position: winit::dpi::PhysicalPosition<f64>) {
        self.cursor = core::mouse::Cursor::Available(iced_winit::conversion::cursor_position(
            position,
            self.viewport.scale_factor(),
        ));
    }
}
