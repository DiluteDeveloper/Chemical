use crate::{Renderer, renderer::line_renderer::LineDescriptor};

use glam::camera::rh::{
    proj::directx,
    view::{self, look_at_mat4},
};
use log::info;
use std::sync::Arc;
use winit::{
    dpi::PhysicalPosition,
    event::KeyEvent,
    event::WindowEvent,
    event_loop::ActiveEventLoop,
    event_loop::EventLoopProxy,
    keyboard::{KeyCode, PhysicalKey},
    window::Window,
};

#[derive(Debug, PartialEq)]
pub(super) enum CameraMode {
    NoCameraControl,
    FPSCameraControl,
}

#[derive(Debug)]
pub(super) enum ChemicalEvent {
    ChangeCameraMode(CameraMode),
}

use crate::{Camera, CameraController};
use anyhow::anyhow;
use chemical_engine::{
    geometry::{cube, model, plane, sphere},
    scene::{
        SceneContainer,
        types::{DirectionalLight, Mesh, Transform},
    },
    utility::FPSCounter,
};

pub(super) struct ChemicalEngine {
    renderer: Renderer,
    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    camera: Camera,
    camera_controller: CameraController,
    camera_mode: CameraMode,
    fps_counter: FPSCounter,
    window_center: PhysicalPosition<f32>,

    scene: SceneContainer,
}

impl ChemicalEngine {
    pub(super) fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> anyhow::Result<Self> {
        let window_size = window.inner_size();
        let mut renderer = pollster::block_on(Renderer::new(Arc::clone(&window)))
            .map_err(|e| anyhow!("Failed to initialise renderer: {}", e))?;

        let mut camera = Camera::new(
            window_size.width as f32 / window_size.height as f32,
            90.0,
            0.01,
            10000.00,
        );
        camera.transform.position.z = 5.0;

        let window_center = (
            window_size.width as f32 / 2.0,
            window_size.height as f32 / 2.0,
        )
            .into();

        renderer.line_renderer.create_line(&LineDescriptor {
            data: Some(vec![
                (-7.0, 0.0, 7.0).into(),
                (7.0, 0.0, 7.0).into(),
                (7.0, 0.0, -7.0).into(),
                (-7.0, 0.0, -7.0).into(),
                (-7.0, 0.0, 7.0).into(),
            ]),
            width: 0.2,
            colour: wgpu::Color {
                r: 1.0,
                g: 0.5,
                b: 0.5,
                a: 1.0,
            },
        });
        let mut scene = SceneContainer::new();
        chemical_scripting_integrator::entry_point::start(&mut scene);
        renderer.process_scene_operations(&mut scene);
        scene.clear_operations();

        event_loop_proxy
            .as_ref()
            .unwrap()
            .send_event(ChemicalEvent::ChangeCameraMode(CameraMode::NoCameraControl))
            .unwrap();
        Ok(ChemicalEngine {
            renderer: renderer,
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window: window,
            camera_mode: CameraMode::NoCameraControl,
            camera: camera,
            camera_controller: CameraController::new(1.0, 0.002, 0.07),
            fps_counter: FPSCounter::new(),
            window_center: window_center,
            scene,
            //universe_simulation: UniverseSimulation::new(),
        })
    }

    pub(super) fn update(&mut self) {
        if self.camera_mode == CameraMode::FPSCameraControl {
            self.camera_controller
                .update_camera(&mut self.camera, self.fps_counter.delta as f32);
        }
        chemical_scripting_integrator::entry_point::update(&mut self.scene);
        self.renderer.process_scene_operations(&mut self.scene);
        self.scene.clear_operations();
        self.fps_counter.update();
        /*if let Some(fps) = self.fps_counter.fps {
            info!("{}", fps);
        }*/
    }
    pub(super) fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => {
                self.renderer.resize(size.width, size.height);
                self.camera.update_projection(
                    size.width as f32 / size.height as f32,
                    90.0,
                    0.01,
                    10000.00,
                );
            }
            WindowEvent::RedrawRequested => {
                self.update();

                match self.renderer.render(&self.camera) {
                    Ok(_) => {}
                    // Reconfigure the surface if it's lost or outdated
                    Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                        let size = self.window.inner_size();
                        self.renderer.resize(size.width, size.height);
                        self.window_center = (size.width, size.height).into();
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
                    (KeyCode::KeyX, true) => match self.camera_mode {
                        CameraMode::FPSCameraControl => {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::ChangeCameraMode(
                                    CameraMode::NoCameraControl,
                                ))
                                .unwrap();
                        }
                        CameraMode::NoCameraControl => {
                            self.event_loop_proxy
                                .send_event(ChemicalEvent::ChangeCameraMode(
                                    CameraMode::FPSCameraControl,
                                ))
                                .unwrap();
                        }
                    },
                    (KeyCode::F5, true) => {
                        #[cfg(feature = "chemical-gui")]
                        chemical_gui::entry_point::launch_game();
                    }
                    (KeyCode::F4, true) => {
                        #[cfg(feature = "chemical-gui")]
                        chemical_gui::entry_point::build_game();
                    }
                    _ => (),
                }
                self.camera_controller.handle_key(*code, is_pressed);
            }
            WindowEvent::CursorMoved { position, .. } => {
                let dx = position.x as f32 - self.window_center.x;
                let dy = position.y as f32 - self.window_center.y;

                self.camera_controller.handle_mouse_moved(&(dx, dy));

                if self.camera_mode == CameraMode::FPSCameraControl {
                    self.window
                        .set_cursor_position(self.window_center)
                        .expect("Failed to set cursor position to center");
                }
            }
            _ => {}
        }
    }
    pub(super) fn chemical_event(&mut self, event: &ChemicalEvent, _event_loop: &ActiveEventLoop) {
        match event {
            ChemicalEvent::ChangeCameraMode(mode) => match mode {
                CameraMode::FPSCameraControl => {
                    self.window.set_cursor_visible(false);
                    self.camera_mode = CameraMode::FPSCameraControl;
                }
                CameraMode::NoCameraControl => {
                    self.window.set_cursor_visible(true);
                    self.camera_mode = CameraMode::NoCameraControl;
                }
            },
        }
    }
    pub(super) fn device_event(
        &mut self,
        _event: &winit::event::DeviceEvent,
        _event_loop: &ActiveEventLoop,
    ) {
        match _event {
            // DeviceEvent::MouseMotion { delta, .. } => {
            //     self.camera_controller.handle_mouse_moved(&delta);
            // }
            _ => {}
        }
    }
}
