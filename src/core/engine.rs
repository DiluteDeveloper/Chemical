use crate::core::renderer::Renderer;
use std::sync::Arc;
use winit::{
    event::DeviceEvent,
    event::KeyEvent,
    event::WindowEvent,
    event_loop::ActiveEventLoop,
    event_loop::EventLoopProxy,
    keyboard::{KeyCode, PhysicalKey},
    window::{CursorGrabMode, Window},
};

#[derive(Debug)]
pub(super) enum ChemicalEvent {
    MouseLockAndHide,
    MouseUnlockAndShow,
}

pub(super) struct ChemicalEngine {
    renderer: Renderer,
    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    is_in_gui_mode: bool,
}

impl ChemicalEngine {
    pub(super) fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> Option<Self> {
        Some(ChemicalEngine {
            renderer: pollster::block_on(Renderer::new(Arc::clone(&window)))
                .expect("Failed to initialise Renderer"),
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window: window,
            is_in_gui_mode: true,
        })
    }

    pub(super) fn update(&mut self) {
        self.renderer.update();
    }
    pub(super) fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => self.renderer.resize(size.width, size.height),
            WindowEvent::RedrawRequested => {
                self.update();
                match self.renderer.render() {
                    Ok(_) => {}
                    // Reconfigure the surface if it's lost or outdated
                    Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                        let size = self.window.inner_size();
                        self.renderer.resize(size.width, size.height);
                    }
                    Err(e) => {
                        log::error!("Render loop failed: {}", e);
                    }
                }
            }
            WindowEvent::KeyboardInput {
                event:
                    KeyEvent {
                        physical_key: PhysicalKey::Code(code),
                        state: key_state,
                        ..
                    },
                ..
            } => {
                let is_pressed = key_state.is_pressed();
                self.renderer.handle_key(event_loop, *code, is_pressed);
                match (code, is_pressed) {
                    (KeyCode::KeyX, true) => {
                        if self.is_in_gui_mode {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::MouseUnlockAndShow)
                                .unwrap();
                        } else {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::MouseLockAndHide)
                                .unwrap();
                        }
                        self.is_in_gui_mode = !self.is_in_gui_mode;
                    }
                    _ => (),
                }
            }

            _ => {}
        }
    }
    pub(super) fn chemical_event(&mut self, event: &ChemicalEvent, _event_loop: &ActiveEventLoop) {
        match event {
            ChemicalEvent::MouseLockAndHide => {
                self.window.set_cursor_visible(false);
                let _ = self.window.set_cursor_grab(CursorGrabMode::Confined);
                self.renderer.camera_controller.is_enabled = true;
            }
            ChemicalEvent::MouseUnlockAndShow => {
                self.window.set_cursor_visible(true);
                let _ = self.window.set_cursor_grab(CursorGrabMode::None);
                self.renderer.camera_controller.is_enabled = false;
            }
        }
    }
    pub(super) fn device_event(
        &mut self,
        event: &winit::event::DeviceEvent,
        event_loop: &ActiveEventLoop,
    ) {
        match event {
            DeviceEvent::MouseMotion { delta, .. } => {
                self.renderer.handle_mouse_moved(event_loop, &delta);
            }
            _ => {}
        }
    }
}
