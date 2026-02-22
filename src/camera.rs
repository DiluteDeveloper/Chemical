pub struct Camera {
    pub position: cgmath::Point3<f32>,
    pub rotation: cgmath::Euler<cgmath::Rad<f32>>,

    proj: cgmath::Matrix4<f32>,
}

use cgmath::EuclideanSpace;

impl Camera {
    fn to_matrix(&self) -> Option<cgmath::Matrix4<f32>> {
        let translation: cgmath::Matrix4<f32> =
            cgmath::Matrix4::from_translation(self.position.to_vec());

        let view = (translation * cgmath::Matrix4::from(self.rotation)).invert()?;

        return Some(OPENGL_TO_WGPU_MATRIX * self.proj * view);
    }

    pub fn new(aspect: f32, fov: f32, znear: f32, zfar: f32) -> Self {
        Self {
            position: (0.0, 0.0, 0.0).into(),
            rotation: cgmath::Euler::new(cgmath::Rad(0.0), cgmath::Rad(0.0), cgmath::Rad(0.0)),
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

// We need this for Rust to store our data correctly for the shaders
#[repr(C)]
// This is so we can store this in a buffer
#[derive(Debug, Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
pub struct CameraUniform {
    // We can't use cgmath with bytemuck directly, so we'll have
    // to convert the Matrix4 into a 4x4 f32 array
    view_proj: [[f32; 4]; 4],
}

impl CameraUniform {
    pub fn new() -> Self {
        use cgmath::SquareMatrix;
        Self {
            view_proj: cgmath::Matrix4::identity().into(),
        }
    }

    pub fn update_view_proj(&mut self, camera: &Camera) {
        self.view_proj = camera.to_matrix().unwrap().into();
    }
}
pub struct CameraController {
    speed: f32,
    is_forward_pressed: bool,
    is_backward_pressed: bool,
    is_left_pressed: bool,
    is_right_pressed: bool,
}

use cgmath::{InnerSpace, SquareMatrix};
use winit::keyboard::KeyCode;

impl CameraController {
    pub fn new(speed: f32) -> Self {
        Self {
            speed,
            is_forward_pressed: false,
            is_backward_pressed: false,
            is_left_pressed: false,
            is_right_pressed: false,
        }
    }

    pub fn handle_key(&mut self, code: KeyCode, is_pressed: bool) -> bool {
        match code {
            KeyCode::KeyW | KeyCode::ArrowUp => {
                self.is_forward_pressed = is_pressed;
                true
            }
            KeyCode::KeyA | KeyCode::ArrowLeft => {
                self.is_left_pressed = is_pressed;
                true
            }
            KeyCode::KeyS | KeyCode::ArrowDown => {
                self.is_backward_pressed = is_pressed;
                true
            }
            KeyCode::KeyD | KeyCode::ArrowRight => {
                self.is_right_pressed = is_pressed;
                true
            }
            _ => false,
        }
    }

    pub fn update_camera(&self, camera: &mut Camera) {
        // Convert Euler to 3x3 rotation matrix
        let rot_matrix: cgmath::Matrix3<f32> = cgmath::Matrix3::from(camera.rotation);

        // Apply rotation to local forward vector
        let forward = rot_matrix * cgmath::Vector3::new(0.0, 0.0, -1.0);
        let forward_norm = forward.normalize_to(1.0);
        let forward_mag = forward.magnitude();
        let up = rot_matrix * cgmath::Vector3::new(0.0, 1.0, 0.0);
        let up_norm = up.normalize_to(1.0);

        // Prevents glitching when the camera gets too close to the
        // center of the scene.
        if self.is_forward_pressed && forward_mag > self.speed {
            camera.position += forward_norm * self.speed;
        }
        if self.is_backward_pressed {
            camera.position -= forward_norm * self.speed;
        }

        let right = forward_norm.cross(up_norm);

        if self.is_right_pressed {
            // Rescale the distance between the target and the eye so
            // that it doesn't change. The eye, therefore, still
            // lies on the circle made by the target and eye.
            camera.position += (right * self.speed).normalize() * 0.01;
        }
        if self.is_left_pressed {
            camera.position -= (right * self.speed).normalize() * 0.01;
        }
    }
}
