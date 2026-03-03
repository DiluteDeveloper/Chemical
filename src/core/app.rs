use std::sync::Arc;

use winit::{
    application::ApplicationHandler,
    event::*,
    event_loop::{ActiveEventLoop, EventLoopProxy},
    keyboard::{KeyCode, PhysicalKey},
    window::{CursorGrabMode, Window},
};

use crate::state::State;

#[derive(Debug)]
pub enum MyEvent {
    MouseLockAndHide,
    MouseUnlockAndShow,
}

pub struct App {
    proxy: EventLoopProxy<MyEvent>,
    pub(super) window: Option<Arc<Window>>,
    is_mouse_locked: bool,
}

impl App {
    pub fn new(proxy: EventLoopProxy<MyEvent>) -> Self {
        Self {
            state: None,
            proxy: proxy,
            window: None,
            is_mouse_locked: false,
        }
    }
}

use log::info;

impl ApplicationHandler<MyEvent> for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let all_monitors = event_loop.available_monitors().collect::<Vec<_>>();
        let target_monitor = all_monitors.get(0).unwrap();

        for (i, m) in all_monitors.iter().enumerate() {
            info!("Monitor {} DPI scale factor: {:?}", i, m.size());
        }
        let window_attributes = Window::default_attributes()
            .with_title(String::from(format!(
                "ChemicalRS {}",
                env!("CARGO_PKG_VERSION")
            )))
            .with_position(target_monitor.position())
            .with_inner_size(winit::dpi::LogicalSize::new(1280.0, 720.0));

        self.window = Some(Arc::new(
            event_loop.create_window(window_attributes).unwrap(),
        ));

        self.state = Some(
            pollster::block_on(State::new(Arc::clone(self.window.as_ref().unwrap()))).unwrap(),
        );
    }

    fn user_event(&mut self, _event_loop: &ActiveEventLoop, event: MyEvent) {
        let window_arc = self.window.as_ref().unwrap();
        let window = window_arc.as_ref();
        let state = match &mut self.state {
            Some(canvas) => canvas,
            None => return,
        };
        match event {
            MyEvent::MouseLockAndHide => {
                window.set_cursor_visible(false);
                let _ = window.set_cursor_grab(CursorGrabMode::Confined);
                state.camera_controller.is_enabled = true;
            }
            MyEvent::MouseUnlockAndShow => {
                window.set_cursor_visible(true);
                let _ = window.set_cursor_grab(CursorGrabMode::None);
                state.camera_controller.is_enabled = false;
            }
        }
    }

    fn window_event(
        &mut self,
        event_loop: &ActiveEventLoop,
        _window_id: winit::window::WindowId,
        event: WindowEvent,
    ) {
        let state = match &mut self.state {
            Some(canvas) => canvas,
            None => return,
        };

        let window_arc = self.window.as_ref().unwrap();
        let window = window_arc.as_ref();
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => state.resize(size.width, size.height),
            WindowEvent::RedrawRequested => {
                state.update();
                match state.render() {
                    Ok(_) => {}
                    // Reconfigure the surface if it's lost or outdated
                    Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                        let size = window.inner_size();
                        state.resize(size.width, size.height);
                    }
                    Err(e) => {
                        log::error!("Unable to render {}", e);
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
                state.handle_key(event_loop, code, is_pressed);
                match (code, is_pressed) {
                    (KeyCode::KeyX, true) => {
                        if self.is_mouse_locked {
                            self.proxy.send_event(MyEvent::MouseUnlockAndShow).unwrap();
                        } else {
                            self.proxy.send_event(MyEvent::MouseLockAndHide).unwrap();
                        }
                        self.is_mouse_locked = !self.is_mouse_locked;
                    }
                    _ => (),
                }
            }

            _ => {}
        }
    }
    fn device_event(
        &mut self,
        event_loop: &ActiveEventLoop,
        _device_id: winit::event::DeviceId,
        event: DeviceEvent,
    ) {
        let state = match &mut self.state {
            Some(canvas) => canvas,
            None => return,
        };
        match event {
            DeviceEvent::MouseMotion { delta, .. } => {
                state.handle_mouse_moved(event_loop, &delta);
            }
            _ => {}
        }
    }
}
