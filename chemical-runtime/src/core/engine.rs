use chemical_api::{
    Camera, Renderer,
    camera::CameraController,
    renderer::{self, line_renderer::LineDescriptor},
    scene::SceneContainer,
    utility::FPSCounter,
};
use glam::Vec2;

use crate::input;
use anyhow::anyhow;
use std::sync::Arc;
use winit::{
    dpi::{PhysicalPosition, PhysicalSize},
    event::{DeviceEvent, KeyEvent, WindowEvent},
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
    direction: Vec2,
    offset: Vec2,

    #[cfg(feature = "chemical-gui")]
    gui: chemical_gui::ChemicalGUI,
}

impl ChemicalEngine {
    pub fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<ChemicalEvent>>,
    ) -> anyhow::Result<Self> {
        let window_size = window.inner_size();

        let (surface, texture_format, device, queue, adapter, surface_config) = pollster::block_on(
            Renderer::initialize_window_surface(Arc::clone(&window), window_size, false),
        )
        .map_err(|e| anyhow!("Failed to initialise renderer: {}", e))?;

        #[cfg(feature = "chemical-gui")]
        let (gui, viewport_region) = chemical_gui::ChemicalGUI::new(
            &device,
            &queue,
            &adapter,
            &texture_format,
            Arc::clone(&window),
        );

        let render_target = renderer::RenderTarget::Custom(
            PhysicalSize {
                width: viewport_region.width,
                height: viewport_region.height,
            },
            wgpu::Origin3d {
                // x: (1280 / 2) - 360,
                // y: (720 / 2) - 240,
                x: viewport_region.offset_x,
                y: viewport_region.offset_y,
                z: 0,
            },
        );
        let renderer_descriptor = renderer::RendererDescriptor {
            texture_format,
            device,
            queue,
            render_target: render_target,
            surface_config,
            surface,
            // window: Arc::clone(&window),
        };
        let mut renderer = Renderer::new(renderer_descriptor);

        let camera = Camera::new(
            window_size.width as f32 / window_size.height as f32,
            90.0,
            0.01,
            10000.00,
        );

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

        #[cfg(feature = "chemical-scripting")]
        chemical_scripting::entry_point::start(&mut scene);

        renderer.process_scene_operations(&mut scene);
        scene.clear_operations();

        event_loop_proxy
            .as_ref()
            .unwrap()
            .send_event(ChemicalEvent::ChangeCameraMode(CameraMode::NoCameraControl))
            .unwrap();

        Ok(ChemicalEngine {
            renderer,
            #[cfg(feature = "chemical-gui")]
            gui,
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
            window,
            camera_mode: CameraMode::NoCameraControl,
            camera,
            camera_controller: CameraController::new(1.0, 0.002, 0.07),
            fps_counter: FPSCounter::new(),
            window_center,
            scene,
            direction: (1.0, 1.0).into(),
            offset: (0.0, 0.0).into(),
        })
    }

    pub fn update(&mut self) {
        if self.camera_mode == CameraMode::FPSCameraControl {
            self.camera_controller
                .update_camera(&mut self.camera, self.fps_counter.delta as f32);
        }
        #[cfg(feature = "chemical-scripting")]
        chemical_scripting::entry_point::update(&mut self.scene);

        self.renderer.process_scene_operations(&mut self.scene);
        self.scene.clear_operations();
        self.fps_counter.update();

    }
    fn window_resized(&mut self, size: &PhysicalSize<u32>) {
        //self.renderer.resize(size.width, size.height);
        #[cfg(not(feature = "chemical-gui"))]
        self.camera
            .update_projection(size.width as f32 / size.height as f32, 90.0, 0.01, 10000.00);

        self.window_center = (size.width / 2, size.height / 2).into();
        #[cfg(feature = "chemical-gui")]
        self.gui.resize(size);
        self.renderer
            .resize(renderer::RenderTarget::Window(PhysicalSize {
                width: size.width,
                height: size.height,
            }));
    }
    #[cfg(feature = "chemical-gui")]
    fn render_target_moved(&mut self, render_target: &renderer::RenderTarget) {
        self.renderer.resize(render_target.clone());
        match render_target {
            renderer::RenderTarget::Custom(size, offset) => {
                self.camera.update_projection(
                    size.width as f32 / size.height as f32,
                    90.0,
                    0.01,
                    10000.00,
                );
            }
            renderer::RenderTarget::Window(_) => (),
        }
    }
    pub fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => {
                self.window_resized(size);
            }
            WindowEvent::RedrawRequested => {
                self.update();
                let encoder = self.renderer.prepare(&self.camera);

                let (encoder, surface_texture) = match self.renderer.render(&self.camera, encoder) {
                    Ok(texture) => texture,
                    // Reconfigure the surface if it's lost or outdated
                    Err(wgpu::SurfaceError::Lost | wgpu::SurfaceError::Outdated) => {
                        self.window_resized(&self.window.inner_size());
                        return;
                    }
                    Err(e) => {
                        log::error!("Render loop failed: {}", e);
                        return;
                    }
                };
                #[cfg(feature = "chemical-gui")]
                self.gui.redraw(&surface_texture);
                self.renderer.publish(encoder, surface_texture);

                self.window.request_redraw();
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
                    // (KeyCode::F5, true) => {
                    //     #[cfg(feature = "chemical-gui")]
                    //     chemical_gui::entry_point::launch_game();
                    // }
                    // (KeyCode::F4, true) => {
                    //     #[cfg(feature = "chemical-gui")]
                    //     chemical_gui::entry_point::build_game();
                    // }
                    _ => (),
                }
                if let Some(kc) = input::winit_keycode_to_chemical(*code) {
                    self.camera_controller.handle_key(kc, is_pressed);
                }
            }
            WindowEvent::CursorMoved { position, .. } => {
                let dx = position.x as f32 - self.window_center.x;
                let dy = position.y as f32 - self.window_center.y;

                #[cfg(feature = "chemical-gui")]
                self.gui.cursor_moved(*position);

                match self.camera_mode {
                    CameraMode::FPSCameraControl => {
                        self.camera_controller.handle_mouse_moved(&(dx, dy));
                        self.window
                            .set_cursor_position(self.window_center)
                            .expect("Failed to set cursor position to center");
                    }
                    _ => (),
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
            DeviceEvent::MouseMotion { delta, .. } => {
                self.camera_controller
                    .handle_mouse_moved(&(delta.0 as f32, delta.1 as f32));
            }
            _ => {}
        }
    }
}
