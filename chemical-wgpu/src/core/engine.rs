use crate::Renderer;
use crate::renderer::mesh_renderer::lighting::PointLight;
use chemical_engine::geometry::sphere;

use chemical_engine::scene::SceneContainer;
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
    scene::types::{IndexMesh, Transform},
    utility::FPSCounter,
};

pub(super) struct ChemicalEngine {
    renderer: Renderer,
    event_loop_proxy: EventLoopProxy<ChemicalEvent>,
    window: Arc<Window>,

    //universe_simulation: UniverseSimulation,
    camera: Camera,
    camera_controller: CameraController,
    camera_mode: CameraMode,
    fps_counter: FPSCounter,
    window_center: PhysicalPosition<f32>,

    scene: SceneContainer,
}

const UNIVERSE_SIMULATION_ORBIT_TRAIL_RESOLUTION: u64 = 3000;

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

        let (vertices, indices) = sphere::generate_index_sphere(200)
            .map_err(|e| anyhow!("Failed to generate index sphere: {}", e))
            .expect("Tried to create invalid index sphere for celestial body mesh!");

        let transform = Transform {
            position: (0.0, 0.0, 0.0).into(),
            scale: (3.0, 3.0, 3.0).into(),
            orientation: (0.0, 0.0, 0.0, -1.0).into(),
        };
        let index_mesh = IndexMesh::new(&vertices, &indices, 1, true, 0);

        let mut scene = SceneContainer::new();
        scene.transform_handler.insert_transform(transform);
        scene.mesh_handler.insert_imesh(index_mesh);

        /*mesh_descriptor.transform_id = renderer.mesh_renderer.create_transform(&transform);
        renderer
            .mesh_renderer
            .create_index_mesh(&mesh_descriptor, &renderer.device);*/
        /*transform.position.x = 5.0;
        mesh_descriptor.transform_id = renderer.mesh_renderer.create_transform(&transform);
        mesh_descriptor.is_lit = true;
        renderer
            .mesh_renderer
            .create_index_mesh(&mesh_descriptor, &renderer.device);
        transform.position.x = 0.0;
        transform.position.z = 7.0;
        transform.position.y = 3.0;
        mesh_descriptor.transform_id = renderer.mesh_renderer.create_transform(&transform);
        mesh_descriptor.is_lit = true;
        renderer
            .mesh_renderer
            .create_index_mesh(&mesh_descriptor, &renderer.device);

        vertices = geometry::cube::generate_vertex_cube((1.0, 1.0, 1.0).into()).to_vec();

        let mut cube_mesh_descriptor = VertexMeshDescriptor {
            vertices: vertices,
            num_instances: 1,
            transform_id: 0,
            is_lit: true,
        };
        transform.position.x = -5.0;
        transform.position.z = 0.0;
        transform.position.y = 5.0;
        cube_mesh_descriptor.transform_id = renderer.mesh_renderer.create_transform(&transform);
        renderer
            .mesh_renderer
            .create_vertex_mesh(&cube_mesh_descriptor, &renderer.device);

        vertices = geometry::plane::generate_vertex_plane((30.0, 30.0).into()).to_vec();

        let mut plane_mesh_descriptor = VertexMeshDescriptor {
            vertices: vertices,
            num_instances: 1,
            transform_id: 0,
            is_lit: true,
        };
        transform.scale = (1.0, 1.0, 1.0).into();
        transform.position.x = 0.0;
        transform.position.y = -2.5;
        plane_mesh_descriptor.transform_id = renderer.mesh_renderer.create_transform(&transform);
        renderer
            .mesh_renderer
            .create_vertex_mesh(&plane_mesh_descriptor, &renderer.device);

        renderer
            .mesh_renderer
            .light_storage
            .add_point_light(&PointLight::new([20.0, 20.0, 20.0], [0.5, 1.0, 0.8], 50.0))
            .unwrap();*/

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
            camera_controller: CameraController::new(1.0, 0.0002, 0.07),
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

        // self.simulation_timer += self.fps_counter.delta as f32;
        // if self.simulation_timer > 0.0 {
        //     self.simulation_timer = 0.0;
        //for _i in 0..UNIVERSE_SIMULATION_ORBIT_TRAIL_RESOLUTION {
        //self.universe_simulation.tick(self.fps_counter.delta);
        //}
        // }

        /*for (i, body) in self.universe_simulation.celestial_bodies.iter().enumerate() {
            if self.universe_simulation.get_tick_count()
                % UNIVERSE_SIMULATION_ORBIT_TRAIL_RESOLUTION
                == 0
            {
                let line = self.renderer.line_renderer.get_line(i).unwrap();
                line.push(&[body.position]);
            }
            self.renderer
                .mesh_renderer
                .get_transform(i)
                .unwrap()
                .position = body.position;
        }*/

        self.renderer.process_scene_operations(&mut self.scene);
        self.scene.clear_operations();
        self.fps_counter.update();
    }
    pub(super) fn window_event(&mut self, event: &WindowEvent, event_loop: &ActiveEventLoop) {
        match event {
            WindowEvent::CloseRequested => event_loop.exit(),
            WindowEvent::Resized(size) => self.renderer.resize(size.width, size.height),
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
