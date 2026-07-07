use crate::rendering::{
    line_renderer::{self, LineRenderer},
    renderer::Renderer,
    universe_renderer::UniverseRenderer,
};
use chemical_engine::physics::universe::simulation::UniverseSimulation;
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

use crate::camera::{Camera, CameraController};
use crate::utility::fps_counter::FPSCounter;

pub(super) struct ChemicalEngine {
    renderer: Renderer,
    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    universe_simulation: UniverseSimulation,
    universe_renderer: UniverseRenderer,

    line_renderer: LineRenderer,
    line: line_renderer::Line,
    line2: line_renderer::Line,
    line3: line_renderer::Line,
    line4: line_renderer::Line,
    line5: line_renderer::Line,

    camera: Camera,
    camera_controller: CameraController,
    camera_mode: CameraMode,
    fps_counter: FPSCounter,
    window_center: PhysicalPosition<f32>,
}

use anyhow::anyhow;
impl ChemicalEngine {
    pub(super) fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> anyhow::Result<Self> {
        let window_size = window.inner_size();
        let renderer = pollster::block_on(Renderer::new(Arc::clone(&window)))
            .map_err(|e| anyhow!("Failed to initialise renderer: {}", e))?;

        let mut camera = Camera::new(
            window_size.width as f32 / window_size.height as f32,
            90.0,
            0.1,
            100000.00,
        );
        camera.transform.position.z = 5.0;

        let window_center = (
            window_size.width as f32 / 2.0,
            window_size.height as f32 / 2.0,
        )
            .into();

        let mut sim = UniverseSimulation::new();
        for i in 0..1000000 {
            sim.tick();
        }
        let mut line = line_renderer::Line::new((sim.body_a.trail.len() - 1) as u32, &renderer);
        line.fill(&renderer, &sim.body_a.trail);
        let mut line2 = line_renderer::Line::new((sim.body_b.trail.len() - 1) as u32, &renderer);
        line2.fill(&renderer, &sim.body_b.trail);
        let mut line3 = line_renderer::Line::new((sim.body_c.trail.len() - 1) as u32, &renderer);
        line3.fill(&renderer, &sim.body_c.trail);
        let mut line4 = line_renderer::Line::new((sim.body_d.trail.len() - 1) as u32, &renderer);
        line4.fill(&renderer, &sim.body_d.trail);
        let mut line5 = line_renderer::Line::new((sim.body_e.trail.len() - 1) as u32, &renderer);
        line5.fill(&renderer, &sim.body_e.trail);

        event_loop_proxy
            .as_ref()
            .unwrap()
            .send_event(ChemicalEvent::ChangeCameraMode(CameraMode::NoCameraControl))
            .unwrap();
        Ok(ChemicalEngine {
            line_renderer: LineRenderer::new(&renderer, &sim.body_a.trail),
            line: line,
            line2: line2,
            line3: line3,
            line4: line4,
            line5: line5,
            universe_renderer: UniverseRenderer::new(&renderer),
            renderer: renderer,
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window: window,
            camera_mode: CameraMode::NoCameraControl,
            camera: camera,
            camera_controller: CameraController::new(60.0, 0.0002, 0.07),
            fps_counter: FPSCounter::new(),
            window_center: window_center,
            universe_simulation: UniverseSimulation::new(),
        })
    }

    pub(super) fn update(&mut self) {
        // info!(
        //     "FPS: {}, Delta: {}",
        //     self.fps_counter.fps.unwrap_or(-1),
        //     self.fps_counter.delta
        // );

        if self.camera_mode == CameraMode::FPSCameraControl {
            self.camera_controller
                .update_camera(&mut self.camera, self.fps_counter.delta as f32);
        }

        self.universe_simulation.tick();

        self.universe_renderer.update_celestial_body_meshes(
            &self.renderer,
            &self.universe_simulation.body_a,
            &self.universe_simulation.body_b,
            &self.universe_simulation.body_c,
            &self.universe_simulation.body_d,
            &self.universe_simulation.body_e,
        );
        let camera_matrix = self
            .camera
            .get_transformation_matrix()
            .expect("Failed to get camera transformation matrix");

        self.universe_renderer
            .upload_camera_matrix(&self.renderer, &camera_matrix.into());

        self.line_renderer
            .upload_camera_matrix(&self.renderer, &camera_matrix.into());

        self.line_renderer
            .upload_camera_position(&self.renderer, &self.camera.transform.position.into());

        self.fps_counter.update();
    }
    pub(super) fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => self.renderer.resize(size.width, size.height),
            WindowEvent::RedrawRequested => {
                self.update();

                self.line_renderer.render(
                    &mut self.renderer,
                    &[
                        &self.line,
                        &self.line2,
                        &self.line3,
                        &self.line4,
                        &self.line5,
                    ],
                );
                /*match self.universe_renderer.render(&mut self.renderer) {
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
                }*/
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
