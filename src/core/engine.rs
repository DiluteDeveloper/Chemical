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

use crate::camera::{Camera, CameraController};
use crate::utility::fps_counter::FPSCounter;

pub(super) struct ChemicalEngine {
    renderer: Renderer,
    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    camera: Camera,
    camera_controller: CameraController,
    is_in_gui_mode: bool,
    fps_counter: FPSCounter,
}

use log::info;
impl ChemicalEngine {
    pub(super) fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> Option<Self> {
        let window_size = window.inner_size();
        info!("{:?}", window_size);
        Some(ChemicalEngine {
            renderer: pollster::block_on(Renderer::new(Arc::clone(&window)))
                .expect("Failed to initialise Renderer"),
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window: window,
            is_in_gui_mode: true,
            camera: Camera::new(
                window_size.width as f32 / window_size.height as f32,
                45.0,
                0.1,
                100.00,
            ),
            camera_controller: CameraController::new(0.02, 0.006, 0.2),
            fps_counter: FPSCounter::new(),
        })
    }

    pub(super) fn update(&mut self) {
        self.camera_controller.update_camera(&mut self.camera);
        self.camera.upload_to_renderer(&mut self.renderer);
        self.fps_counter.update();
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
                match (code, is_pressed) {
                    (KeyCode::Escape, true) => event_loop.exit(),
                    (KeyCode::KeyX, true) => {
                        if self.is_in_gui_mode {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::MouseLockAndHide)
                                .unwrap();
                        } else {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::MouseUnlockAndShow)
                                .unwrap();
                        }
                        self.is_in_gui_mode = !self.is_in_gui_mode;
                    }
                    _ => (),
                }
                if self.camera_controller.is_enabled {
                    self.camera_controller.handle_key(*code, is_pressed);
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
                self.camera_controller.is_enabled = true;
            }
            ChemicalEvent::MouseUnlockAndShow => {
                self.window.set_cursor_visible(true);
                let _ = self.window.set_cursor_grab(CursorGrabMode::None);
                self.camera_controller.is_enabled = false;
            }
        }
    }
    pub(super) fn device_event(
        &mut self,
        event: &winit::event::DeviceEvent,
        _event_loop: &ActiveEventLoop,
    ) {
        match event {
            DeviceEvent::MouseMotion { delta, .. } => {
                self.camera_controller.handle_mouse_moved(&delta);
            }
            _ => {}
        }
    }
}
