pub struct Camera {
    pub position: cgmath::Point3<f32>,
    pub orientation: cgmath::Quaternion<f32>,

    proj: cgmath::Matrix4<f32>,
}

use cgmath::{EuclideanSpace, InnerSpace, Rad, Rotation, Rotation3, SquareMatrix};

impl Camera {
    pub fn to_matrix(&self) -> Option<cgmath::Matrix4<f32>> {
        let translation_matrix: cgmath::Matrix4<f32> =
            cgmath::Matrix4::from_translation(self.position.to_vec());
        // let rotation_matrix = cgmath::Matrix4::look_to_rh(
        //     self.position,
        //     self.orientation.rotate_vector(-Vector3::unit_z()),
        //     self.orientation.rotate_vector(Vector3::unit_y()),
        // );
        let rotation_matrix = cgmath::Matrix4::from(self.orientation);
        let transformation_matrix = translation_matrix * rotation_matrix;
        // info!(
        //     "trans: {:?}, rot: {:?}, final: {:?}",
        //     translation_matrix, rotation_matrix, transformation_matrix
        // );
        // let transformation_matrix = cgmath::Matrix4::look_to_rh(
        //     self.position,
        //     self.orientation.rotate_vector(-Vector3::unit_z()),
        //     self.orientation.rotate_vector(Vector3::unit_y()),
        // );
        // info!("q: {:?}", self.orientation);
        // info!("t: {:?}", transformation_matrix);

        return Some(OPENGL_TO_WGPU_MATRIX * self.proj * transformation_matrix.invert()?);
    }

    pub fn new(aspect: f32, fov: f32, znear: f32, zfar: f32) -> Self {
        Self {
            position: (0.0, 0.0, 0.0).into(),
            orientation: (0.0, 0.0, 0.0, -1.0).into(),
            proj: cgmath::perspective(cgmath::Deg(fov), aspect, znear, zfar),
        }
    }
}

#[rustfmt::skip]
pub const OPENGL_TO_WGPU_MATRIX: cgmath::Matrix4<f32> = cgmath::Matrix4::from_cols(
    cgmath::Vector4::new(1.0, 0.0, 0.0, 0.0),
    cgmath::Vector4::new(0.0, 1.0, 0.0, 0.0),
    cgmath::Vector4::new(0.0, 0.0, 0.5, 0.0),
    cgmath::Vector4::new(0.0, 0.0, 0.5, 1.0),
);

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
        let fwd = camera.orientation.rotate_vector(-cgmath::Vector3::unit_z());
        let right = camera.orientation.rotate_vector(cgmath::Vector3::unit_x());
        let up = cgmath::Vector3::unit_y();

        let yaw_delta = cgmath::Quaternion::from_axis_angle(
            cgmath::Vector3::unit_y(),
            Rad(-self.yaw_delta * self.look_sensitivity),
        );

        let pitch_delta = cgmath::Quaternion::from_axis_angle(
            right,
            Rad(-self.pitch_delta * self.look_sensitivity),
        );
        self.pitch_delta = 0.0;
        self.yaw_delta = 0.0;
        // let right = cgmath::Vector3::new(right.x, 0.0, right.z).normalize();
        // let fwd = cgmath::Vector3::new(fwd.x, 0.0, fwd.z).normalize();
        // info!("pitch: {:?}", self.pitch_delta);
        //
        // info!(
        //     "pitch: {:?}, yaw: {:?}, or: {:?}",
        //     pitch_delta, yaw_delta, camera.orientation
        // );
        camera.orientation = (yaw_delta * pitch_delta * camera.orientation).normalize();
        // info!("Should be normalized fwd: {:?}", fwd);
        // info!("Should be normalized q: {:?}", camera.orientation);
        // info!("Should be normalized up : {:?}", up);
        // info!("Should be normalized right: {:?}", right);

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
        camera.position += self.velocity;
    }
}
