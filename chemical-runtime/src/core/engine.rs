use chemical_api::{Renderer, renderer::line_renderer::LineDescriptor};
#[cfg(feature = "chemical-gui")]
use chemical_gui::test::Counter;

use std::sync::{Arc, Mutex};
use winit::{
    dpi::{PhysicalPosition, PhysicalSize},
    event::{KeyEvent, WindowEvent},
    event_loop::{ActiveEventLoop, EventLoopProxy},
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

use anyhow::anyhow;
use chemical_api::{Camera, camera::CameraController};
use chemical_api::{scene::SceneContainer, utility::FPSCounter};

pub(super) struct ChemicalEngine {
    renderer: Arc<Mutex<Renderer>>,

    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    camera: Arc<Mutex<Camera>>,
    camera_controller: CameraController,
    camera_mode: CameraMode,
    fps_counter: FPSCounter,
    window_center: PhysicalPosition<f32>,

    scene: SceneContainer,

    #[cfg(feature = "chemical-gui")]
    gui: chemical_gui::ChemicalGUI,
}

impl ChemicalEngine {
    pub fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> anyhow::Result<Self> {
        let window_size = window.inner_size();
        let mut renderer = Arc::new(Mutex::new(
            pollster::block_on(Renderer::new(Arc::clone(&window)))
                .map_err(|e| anyhow!("Failed to initialise renderer: {}", e))?,
        ));

        let mut camera = Arc::new(Mutex::new(Camera::new(
            window_size.width as f32 / window_size.height as f32,
            90.0,
            0.01,
            10000.00,
        )));

        let window_center = (
            window_size.width as f32 / 2.0,
            window_size.height as f32 / 2.0,
        )
            .into();

        // renderer.line_renderer.create_line(&LineDescriptor {
        //     data: Some(vec![
        //         (-7.0, 0.0, 7.0).into(),
        //         (7.0, 0.0, 7.0).into(),
        //         (7.0, 0.0, -7.0).into(),
        //         (-7.0, 0.0, -7.0).into(),
        //         (-7.0, 0.0, 7.0).into(),
        //     ]),
        //     width: 0.2,
        //     colour: wgpu::Color {
        //         r: 1.0,
        //         g: 0.5,
        //         b: 0.5,
        //         a: 1.0,
        //     },
        // });
        let mut scene = SceneContainer::new();

        #[cfg(feature = "chemical-scripting")]
        chemical_scripting::entry_point::start(&mut scene);

        renderer
            .lock()
            .unwrap()
            .process_scene_operations(&mut scene);
        scene.clear_operations();

        event_loop_proxy
            .as_ref()
            .unwrap()
            .send_event(ChemicalEvent::ChangeCameraMode(CameraMode::NoCameraControl))
            .unwrap();

        let renderer_guard = renderer.lock().unwrap();

        Ok(ChemicalEngine {
            #[cfg(feature = "chemical-gui")]
            gui: chemical_gui::ChemicalGUI::new(
                &renderer_guard.device,
                &renderer_guard.queue,
                &renderer_guard.adapter,
                &renderer_guard.surface_format,
                &window,
                Counter::new(camera.clone(), renderer.clone()),
            ),
            renderer: renderer.clone(),
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window: window,
            camera_mode: CameraMode::NoCameraControl,
            camera: camera,
            camera_controller: CameraController::new(1.0, 0.002, 0.07),
            fps_counter: FPSCounter::new(),
            window_center: window_center,
            scene,
        })
    }

    pub fn update(&mut self) {
        if self.camera_mode == CameraMode::FPSCameraControl {
            self.camera_controller.update_camera(
                &mut self.camera.lock().unwrap(),
                self.fps_counter.delta as f32,
            );
        }
        #[cfg(feature = "chemical-scripting")]
        chemical_scripting::entry_point::update(&mut self.scene);

        self.renderer
            .lock()
            .unwrap()
            .process_scene_operations(&mut self.scene);
        self.scene.clear_operations();
        self.fps_counter.update();
        /*if let Some(fps) = self.fps_counter.fps {
            info!("{}", fps);
        }*/
    }
    fn window_resized(&mut self, size: &PhysicalSize<u32>) {
        //self.renderer.resize(size.width, size.height);
        self.camera.lock().unwrap().update_projection(
            size.width as f32 / size.height as f32,
            90.0,
            0.01,
            10000.00,
        );
        self.window_center = (size.width, size.height).into();
        #[cfg(feature = "chemical-gui")]
        self.gui.resize(size);
        self.renderer
            .lock()
            .unwrap()
            .resize(size.width, size.height);
    }
    pub fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => {
                self.window_resized(size);
            }
            WindowEvent::RedrawRequested => {
                self.update();
                let tex = {
                    let renderer = self.renderer.lock().unwrap();
                    renderer.surface.get_current_texture().unwrap()
                    // lock released here, at the end of this block — `tex` is now owned, independent of the guard
                };

                self.gui.redraw(&tex); // redraw probably wants a reference, that's fine
                self.window.request_redraw();
                tex.present(); // now valid — consumes the owned SurfaceTexture

                // match self.renderer.render(
                //     &self.camera,
                //     #[cfg(feature = "chemical-gui")]
                //     &mut self.gui,
                // ) {
                //     Ok(_) => {}
                //     // Reconfigure the surface if it's lost or outdated
                //     Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                //         self.window_resized(&self.window.inner_size());
                //     }
                //     Err(e) => {
                //         log::error!("Render loop failed: {}", e);
                //     }
                // }
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
                //self.camera_controller.handle_key(*code, is_pressed);
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
