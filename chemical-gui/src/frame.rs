use std::sync::{Arc, Mutex};

use chemical_api::{Camera, Renderer};
use iced_wgpu::wgpu;
use iced_widget::core::{Rectangle, mouse};
use iced_widget::shader::{self, Viewport};

#[derive(Debug)]
pub struct SceneProgram {
    pub renderer: Arc<Mutex<Renderer>>,
    pub camera: Arc<Mutex<Camera>>,
}

impl<Message> shader::Program<Message> for SceneProgram {
    type State = ();
    type Primitive = RenderFrame;

    fn draw(
        &self,
        _state: &Self::State,
        _cursor: mouse::Cursor,
        _bounds: Rectangle,
    ) -> Self::Primitive {
        RenderFrame {
            renderer: self.renderer.clone(), // cheap — bumps the Arc refcount
            camera: self.camera.clone(),
        }
    }
}

#[derive(Debug)]
pub struct RenderFrame {
    renderer: Arc<Mutex<Renderer>>,
    camera: Arc<Mutex<Camera>>,
}

// Marker type — chemical_api::Renderer already owns its GPU pipelines,
// so there's nothing for iced to cache here. Can't impl shader::Pipeline
// for `()` directly due to Rust's orphan rule (foreign trait + foreign type),
// hence this tiny local wrapper.
#[derive(Debug, Default)]
pub struct NoPipeline;

impl shader::Pipeline for NoPipeline {
    fn new(_device: &wgpu::Device, _queue: &wgpu::Queue, _format: wgpu::TextureFormat) -> Self {
        NoPipeline
    }
}

impl shader::Primitive for RenderFrame {
    type Pipeline = NoPipeline;

    fn prepare(
        &self,
        _pipeline: &mut Self::Pipeline,
        _device: &wgpu::Device,
        _queue: &wgpu::Queue,
        _bounds: &Rectangle,
        _viewport: &Viewport,
    ) {
        self.renderer
            .lock()
            .unwrap()
            .prepare(&self.camera.lock().unwrap());
    }

    fn render(
        &self,
        _pipeline: &Self::Pipeline,
        encoder: &mut wgpu::CommandEncoder,
        target: &wgpu::TextureView,
        _clip_bounds: &Rectangle<u32>,
    ) {
        self.renderer
            .lock()
            .unwrap()
            .render_to_view(encoder, target, &self.camera.lock().unwrap());
    }
}
