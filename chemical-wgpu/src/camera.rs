use crate::utility::TransformNoScale;
use anyhow::anyhow;
use cgmath::{InnerSpace, Matrix4, Quaternion, Rad, Rotation, Rotation3, SquareMatrix, Vector3};
use log::info;

pub struct Camera {
    pub transform: TransformNoScale,

    proj: cgmath::Matrix4<f32>,
}

impl Camera {
    pub fn get_transformation_matrix(&self) -> anyhow::Result<[[f32; 4]; 4]> {
        let transform_matrix: Matrix4<f32> = (&self.transform).into();
        Ok((self.proj
            * transform_matrix
                .invert()
                .ok_or_else(|| anyhow!("Failed to invert transformation matrix"))?)
        .into())
    }

    pub fn new(aspect: f32, fov: f32, znear: f32, zfar: f32) -> Self {
        Self {
            transform: TransformNoScale::default(),
            proj: cgmath::perspective(cgmath::Deg(fov), aspect, znear, zfar),
        }
    }
}

pub struct CameraController {
    acceleration: f32,
    look_sensitivity: f32,
    velocity: cgmath::Vector3<f32>,
    velocity_damping_factor: f32,

    is_forward_pressed: bool,
    is_backward_pressed: bool,
    is_left_pressed: bool,
    is_right_pressed: bool,
    is_up_pressed: bool,
    is_down_pressed: bool,

    pitch_delta: f32,
    yaw_delta: f32,
}

use winit::keyboard::KeyCode;

impl CameraController {
    pub fn new(acceleration: f32, look_sensitivity: f32, velocity_damping_factor: f32) -> Self {
        Self {
            acceleration: acceleration,
            look_sensitivity: look_sensitivity,
            is_forward_pressed: false,
            is_backward_pressed: false,
            is_left_pressed: false,
            is_right_pressed: false,
            is_up_pressed: false,
            is_down_pressed: false,
            velocity: (0.0, 0.0, 0.0).into(),
            velocity_damping_factor: velocity_damping_factor,
            pitch_delta: 0.0,
            yaw_delta: 0.0,
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
    pub fn handle_mouse_moved(&mut self, pos: &(f32, f32)) {
        self.pitch_delta += pos.1;
        self.yaw_delta += pos.0;
    }

    pub fn update_camera(&mut self, camera: &mut Camera, delta: f32) {
        info!("{:?}", camera.transform.orientation);
        let transform = &mut camera.transform;
        let mut fwd = transform.orientation.rotate_vector(-Vector3::unit_z());
        fwd.y = 0.0;
        let right = transform.orientation.rotate_vector(Vector3::unit_x());

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
            self.velocity += fwd * self.acceleration * delta;
        }
        if self.is_backward_pressed {
            self.velocity -= fwd * self.acceleration * delta;
        }
        if self.is_right_pressed {
            self.velocity += right * self.acceleration * delta;
        }
        if self.is_left_pressed {
            self.velocity -= right * self.acceleration * delta;
        }
        if self.is_up_pressed {
            self.velocity += Vector3::unit_y() * self.acceleration * delta;
        }
        if self.is_down_pressed {
            self.velocity -= Vector3::unit_y() * self.acceleration * delta;
        }
        self.velocity += -self.velocity * self.velocity_damping_factor;
        transform.position += self.velocity;
    }
}
