use crate::{geometry::sphere, rendering::mesh::IndexMesh};

use super::renderer::Renderer;
use anyhow::anyhow;
use cgmath::{EuclideanSpace, Matrix4};
use chemical_engine::physics::universe_simulation::celestial_body::CelestialBody;
pub struct UniverseRenderer {
    pub render_pipeline: wgpu::RenderPipeline,
    pub bind_group: wgpu::BindGroup,
    camera_buffer: wgpu::Buffer,
    model_buffer: wgpu::Buffer,
    sphere: IndexMesh,
}

const SHADER_PATH: &str = "res/shaders/universe_shader.wgsl";

impl UniverseRenderer {
    pub fn new(renderer: &Renderer) -> UniverseRenderer {
        let bind_group_layout =
            renderer
                .device
                .create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                    entries: &[
                        wgpu::BindGroupLayoutEntry {
                            binding: 0,
                            visibility: wgpu::ShaderStages::VERTEX,
                            ty: wgpu::BindingType::Buffer {
                                ty: wgpu::BufferBindingType::Uniform,
                                has_dynamic_offset: false,
                                min_binding_size: None,
                            },
                            count: None,
                        },
                        wgpu::BindGroupLayoutEntry {
                            binding: 1,
                            visibility: wgpu::ShaderStages::VERTEX,
                            ty: wgpu::BindingType::Buffer {
                                ty: wgpu::BufferBindingType::Uniform,
                                has_dynamic_offset: false,
                                min_binding_size: None,
                            },
                            count: None,
                        },
                    ],
                    label: Some("bind_group_layout"),
                });

        let camera_buffer = renderer.device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Camera Buffer"),
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let model_buffer = renderer.device.create_buffer(&wgpu::BufferDescriptor {
            label: Some("Model Buffer"),
            size: size_of::<[[[f32; 4]; 4]; 3]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let (vertices, indices) = sphere::generate_index_sphere(30)
            .map_err(|e| anyhow!("Failed to generate index sphere: {}", e))
            .expect("Tried to create invalid index sphere for celestial body mesh!");

        UniverseRenderer {
            render_pipeline: renderer
                .create_default_render_pipeline(
                    SHADER_PATH,
                    &wgpu::BindGroupLayoutDescriptor {
                        entries: &[
                            wgpu::BindGroupLayoutEntry {
                                binding: 0,
                                visibility: wgpu::ShaderStages::VERTEX,
                                ty: wgpu::BindingType::Buffer {
                                    ty: wgpu::BufferBindingType::Uniform,
                                    has_dynamic_offset: false,
                                    min_binding_size: None,
                                },
                                count: None,
                            },
                            wgpu::BindGroupLayoutEntry {
                                binding: 1,
                                visibility: wgpu::ShaderStages::VERTEX,
                                ty: wgpu::BindingType::Buffer {
                                    ty: wgpu::BufferBindingType::Uniform,
                                    has_dynamic_offset: false,
                                    min_binding_size: None,
                                },
                                count: None,
                            },
                        ],
                        label: Some("bind_group_layout"),
                    },
                )
                .expect("Failed to create universe simulation render pipeline!"),
            bind_group: renderer
                .device
                .create_bind_group(&wgpu::BindGroupDescriptor {
                    layout: &bind_group_layout,
                    entries: &[
                        wgpu::BindGroupEntry {
                            binding: 0,
                            resource: camera_buffer.as_entire_binding(),
                        },
                        wgpu::BindGroupEntry {
                            binding: 1,
                            resource: model_buffer.as_entire_binding(),
                        },
                    ],
                    label: Some("universe_bind_group"),
                }),
            camera_buffer: camera_buffer,
            model_buffer: model_buffer,
            sphere: IndexMesh::new_instanced(&vertices, &indices, 3, renderer),
        }
    }
    pub fn upload_camera_matrix(&mut self, renderer: &Renderer, matrix: &[[f32; 4]; 4]) {
        renderer
            .queue
            .write_buffer(&self.camera_buffer, 0, bytemuck::cast_slice(matrix));
    }
    pub fn update_celestial_body_meshes(
        &mut self,
        renderer: &Renderer,
        body_a: &CelestialBody,
        body_b: &CelestialBody,
        body_c: &CelestialBody,
    ) {
        let matrices: [[[f32; 4]; 4]; 3] = [
            Self::celestial_body_to_matrix(body_a).into(),
            Self::celestial_body_to_matrix(body_b).into(),
            Self::celestial_body_to_matrix(body_c).into(),
        ];
        renderer
            .queue
            .write_buffer(&self.model_buffer, 0, bytemuck::cast_slice(&matrices));
    }
    pub fn render(&mut self, renderer: &mut Renderer) -> Result<(), wgpu::SurfaceError> {
        renderer.render(&[&self.sphere], &self.render_pipeline, &self.bind_group)
    }
    fn celestial_body_to_matrix(celestial_body: &CelestialBody) -> cgmath::Matrix4<f32> {
        let v = Matrix4::from_translation(celestial_body.position.to_vec());
        v * Matrix4::from_scale(celestial_body.mass.sqrt())
    }
}
