use crate::core::renderer::{OPENGL_TO_WGPU_MATRIX, Renderer};
use crate::utility::transform::TransformNoScale;
use anyhow::anyhow;
use cgmath::{InnerSpace, Matrix4, Quaternion, Rad, Rotation, Rotation3, SquareMatrix, Vector3};

pub struct Camera {
    transform: TransformNoScale,

    proj: cgmath::Matrix4<f32>,
}

impl Camera {
    pub fn upload_to_renderer(
        &self,
        renderer: &mut Renderer,
        temp_mat: &Matrix4<f32>,
    ) -> anyhow::Result<()> {
        let transform_matrix: Matrix4<f32> = (&self.transform).into();
        let final_matrix = OPENGL_TO_WGPU_MATRIX
            * self.proj
            * transform_matrix
                .invert()
                .ok_or_else(|| anyhow!("Failed to invert transformation matrix"))?
            * temp_mat;

        let renderer_matrix: [[f32; 4]; 4] = final_matrix.into();
        renderer.upload_camera_transformation_matrix(&renderer_matrix);

        Ok(())
    }

    pub fn new(aspect: f32, fov: f32, znear: f32, zfar: f32) -> Self {
        Self {
            transform: TransformNoScale::new_default(),
            proj: cgmath::perspective(cgmath::Deg(fov), aspect, znear, zfar),
        }
    }
}

pub struct CameraController {
    acceleration: f32,
    look_sensitivity: f32,
    velocity: cgmath::Vector3<f32>,
    max_velocity: f32,

    is_forward_pressed: bool,
    is_backward_pressed: bool,
    is_left_pressed: bool,
    is_right_pressed: bool,
    is_up_pressed: bool,
    is_down_pressed: bool,

    pitch_delta: f32,
    yaw_delta: f32,

    pub is_enabled: bool,
}

use winit::keyboard::KeyCode;

impl CameraController {
    pub fn new(acceleration: f32, look_sensitivity: f32, terminal_velocity: f32) -> Self {
        Self {
            acceleration: acceleration,
            look_sensitivity: look_sensitivity,
            max_velocity: terminal_velocity,
            is_forward_pressed: false,
            is_backward_pressed: false,
            is_left_pressed: false,
            is_right_pressed: false,
            is_up_pressed: false,
            is_down_pressed: false,
            velocity: (0.0, 0.0, 0.0).into(),
            pitch_delta: 0.0,
            yaw_delta: 0.0,
            is_enabled: false,
        }
    }

    pub fn handle_key(&mut self, code: KeyCode, is_pressed: bool) {
        match code {
            KeyCode::KeyW | KeyCode::ArrowUp => {
                self.is_forward_pressed = is_pressed;
                return;
            }
            KeyCode::KeyA | KeyCode::ArrowLeft => {
                self.is_left_pressed = is_pressed;
                return;
            }
            KeyCode::KeyS | KeyCode::ArrowDown => {
                self.is_backward_pressed = is_pressed;
                return;
            }
            KeyCode::KeyD | KeyCode::ArrowRight => {
                self.is_right_pressed = is_pressed;
                return;
            }
            KeyCode::Space => {
                self.is_up_pressed = is_pressed;
                return;
            }
            KeyCode::ShiftLeft => {
                self.is_down_pressed = is_pressed;
                return;
            }
            _ => return,
        }
    }
    pub fn handle_mouse_moved(&mut self, pos: &(f64, f64)) {
        self.pitch_delta = pos.1 as f32;
        self.yaw_delta = pos.0 as f32;
    }

    pub fn update_camera(&mut self, camera: &mut Camera) {
        let transform = &mut camera.transform;
        let fwd = transform.orientation.rotate_vector(-Vector3::unit_z());
        let right = transform.orientation.rotate_vector(Vector3::unit_x());
        let up = Vector3::unit_y();

        let yaw_delta = Quaternion::from_axis_angle(
            cgmath::Vector3::unit_y(),
            Rad(-self.yaw_delta * self.look_sensitivity),
        );

        let pitch_delta =
            Quaternion::from_axis_angle(right, Rad(-self.pitch_delta * self.look_sensitivity));
        self.pitch_delta = 0.0;
        self.yaw_delta = 0.0;
        transform.orientation = (yaw_delta * pitch_delta * transform.orientation).normalize();

        if self.is_forward_pressed {
            self.velocity += fwd * self.acceleration;
        }
        if self.is_backward_pressed {
            self.velocity -= fwd * self.acceleration;
        }
        if self.is_right_pressed {
            self.velocity += right * self.acceleration;
        }
        if self.is_left_pressed {
            self.velocity -= right * self.acceleration;
        }
        if self.is_up_pressed {
            self.velocity += up * self.acceleration;
        }
        if self.is_down_pressed {
            self.velocity -= up * self.acceleration;
        }
        self.velocity.x = self
            .velocity
            .x
            .min(self.max_velocity)
            .max(-self.max_velocity);
        self.velocity.y = self
            .velocity
            .y
            .min(self.max_velocity)
            .max(-self.max_velocity);
        self.velocity.z = self
            .velocity
            .z
            .min(self.max_velocity)
            .max(-self.max_velocity);
        self.velocity += -self.velocity * 0.1;
        transform.position += self.velocity;
    }
}
