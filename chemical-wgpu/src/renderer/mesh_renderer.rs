mod geometry;
mod mesh;

use super::Texture;
use crate::Camera;
use crate::utility::Transform;
pub use geometry::sphere;
use geometry::{Index, Vertex};
use mesh::{IndexMesh, VertexMesh};

pub struct IndexMeshDescriptor {
    pub vertices: Vec<Vertex>,
    pub indices: Vec<Index>,
    pub num_instances: u32,
    pub transform_id: TransformID,
}
pub struct VertexMeshDescriptor {
    pub vertices: Vec<Vertex>,
    pub num_instances: u32,
    pub transform_id: TransformID,
}

pub struct MeshRenderer {
    render_pipeline: wgpu::RenderPipeline,
    bind_group: wgpu::BindGroup,
    camera_buffer: wgpu::Buffer,
    model_buffer: wgpu::Buffer,

    vertex_meshes: Vec<(VertexMesh, TransformID)>,
    index_meshes: Vec<(IndexMesh, TransformID)>,
    transforms: Vec<Transform>,

    aligned_model_matrix_size_offset: u64,
}

pub type TransformID = usize;

const SHADER_PATH: &str = "res/shaders/mesh_shader.wgsl";
const MAX_TRANSFORMS: u64 = 100;

impl MeshRenderer {
    pub(super) fn new(device: &wgpu::Device, config: &wgpu::SurfaceConfiguration) -> MeshRenderer {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
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
                        has_dynamic_offset: true,
                        min_binding_size: None,
                    },
                    count: None,
                },
            ],
            label: Some("mesh_renderer_bind_group_layout"),
        });

        let camera_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let aligned_model_matrix_size_offset: u64 = wgpu::util::align_to(
            std::mem::size_of::<[[f32; 4]; 4]>() as u32,
            device.limits().min_uniform_buffer_offset_alignment,
        ) as u64;
        let model_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: aligned_model_matrix_size_offset * MAX_TRANSFORMS,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    binding: 0,
                    resource: camera_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 1,
                    resource: wgpu::BindingResource::Buffer(wgpu::BufferBinding {
                        buffer: &model_buffer,
                        offset: 0,
                        size: wgpu::BufferSize::new(aligned_model_matrix_size_offset),
                    }),
                },
            ],
            label: Some("mesh_renderer_bind_group"),
        });

        let shader_source = std::fs::read_to_string(SHADER_PATH)
            .expect(&format!("Shader path '{}' was invalid!", SHADER_PATH));

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(shader_source.into()),
        });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("mesh_renderer_render_pipeline_layout"),
                bind_group_layouts: &[&bind_group_layout],
                immediate_size: 0,
            });
        let render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("mesh_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[Vertex::layout()], // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &shader,
                entry_point: Some("fs_main"),
                targets: &[Some(wgpu::ColorTargetState {
                    // 4.
                    format: config.format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: None,
                // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                polygon_mode: wgpu::PolygonMode::Fill,
                // Requires Features::DEPTH_CLIP_CONTROL
                unclipped_depth: false,
                // Requires Features::CONSERVATIVE_RASTERIZATION
                conservative: false,
            },
            depth_stencil: Some(wgpu::DepthStencilState {
                format: Texture::DEPTH_FORMAT,
                depth_write_enabled: true,
                depth_compare: wgpu::CompareFunction::Less,
                stencil: wgpu::StencilState::default(),
                bias: wgpu::DepthBiasState::default(),
            }), // 1.
            multisample: wgpu::MultisampleState {
                count: 1,                         // 2.
                mask: !0,                         // 3.
                alpha_to_coverage_enabled: false, // 4.
            },
            multiview_mask: None, // 5.
            cache: None,          // 6.
        };
        let render_pipeline = device.create_render_pipeline(&render_pipeline_descriptor);
        MeshRenderer {
            render_pipeline: render_pipeline,
            bind_group: bind_group,
            camera_buffer: camera_buffer,
            model_buffer: model_buffer,
            vertex_meshes: Vec::new(),
            index_meshes: Vec::new(),
            transforms: Vec::new(),
            aligned_model_matrix_size_offset: aligned_model_matrix_size_offset,
        }
    }
    pub fn create_vertex_mesh(&mut self, descriptor: &VertexMeshDescriptor, device: &wgpu::Device) {
        self.vertex_meshes.push((
            VertexMesh::new(&descriptor.vertices, descriptor.num_instances, &device),
            descriptor.transform_id,
        ));
    }
    pub fn create_index_mesh(&mut self, descriptor: &IndexMeshDescriptor, device: &wgpu::Device) {
        self.index_meshes.push((
            IndexMesh::new(
                &descriptor.vertices,
                &descriptor.indices,
                descriptor.num_instances,
                &device,
            ),
            descriptor.transform_id,
        ));
    }
    pub fn create_transform(&mut self, transform: &Transform) -> TransformID {
        self.transforms.push(*transform);
        self.transforms.len() - 1
    }
    pub fn get_transform(&mut self, transform_id: TransformID) -> &mut Transform {
        self.transforms.get_mut(transform_id).unwrap()
    }
    pub(super) fn render(
        &self,
        camera: &Camera,
        queue: &wgpu::Queue,
        render_pass: &mut wgpu::RenderPass,
    ) {
        render_pass.set_pipeline(&self.render_pipeline);

        queue.write_buffer(
            &self.camera_buffer,
            0,
            bytemuck::bytes_of(&camera.get_transformation_matrix().unwrap()),
        );

        for (i, transform) in self.transforms.iter().enumerate() {
            let t: [[f32; 4]; 4] = transform.into();
            queue.write_buffer(
                &self.model_buffer,
                i as u64 * self.aligned_model_matrix_size_offset,
                bytemuck::bytes_of(&t),
            );
        }

        for (mesh, transform_id) in self.vertex_meshes.iter() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(*transform_id as u64 * self.aligned_model_matrix_size_offset) as u32],
            );

            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }
        for (mesh, transform_id) in self.index_meshes.iter() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(*transform_id as u64 * self.aligned_model_matrix_size_offset) as u32],
            );
            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }
    }
}
