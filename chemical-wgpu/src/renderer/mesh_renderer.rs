pub mod geometry;
pub mod lighting;
mod mesh;
mod renderable;

use super::Texture;
use crate::Camera;
use log::info;

use chemical_engine::scene::{
    type_handlers::{
        light_handler::LightOperationListener, mesh_handler::MeshOperationListener,
        transform_handler::TransformOperationListener,
    },
    types::{IndexMesh, ObjectID, PointLight, Transform, VertexMesh, vertex_mesh::Vertex},
};

use geometry::vertex;
use lighting::LightStorage;
use mesh::{BakedIndexMesh, BakedVertexMesh};
pub use renderable::Renderable;

pub struct MeshRenderer {
    lit_render_pipeline: wgpu::RenderPipeline,
    unlit_render_pipeline: wgpu::RenderPipeline,
    bind_group: wgpu::BindGroup,
    camera_buffer: wgpu::Buffer,
    camera_pos_buffer: wgpu::Buffer,
    light_mat_buffer: wgpu::Buffer,

    model_buffer: wgpu::Buffer,
    model_byte_offset: u64,

    lit_vertex_meshes: Vec<BakedVertexMesh>,
    lit_index_meshes: Vec<BakedIndexMesh>,

    unlit_vertex_meshes: Vec<BakedVertexMesh>,
    unlit_index_meshes: Vec<BakedIndexMesh>,

    //pub light_storage: LightStorage,

    //shadow_renderer: ShadowRenderer,
    queue: wgpu::Queue,
    device: wgpu::Device,
}

const UNLIT_SHADER_PATH: &str = "res/shaders/unlit_shader.wgsl";
const LIT_SHADER_PATH: &str = "res/shaders/lit_shader.wgsl";
const MAX_TRANSFORMS: u64 = 1000;

impl MeshRenderer {
    pub(super) fn new(
        device: &wgpu::Device,
        config: &wgpu::SurfaceConfiguration,
        queue: &wgpu::Queue,
        surface_caps: &wgpu::SurfaceCapabilities,
    ) -> Self {
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
                wgpu::BindGroupLayoutEntry {
                    binding: 2,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Storage { read_only: true },
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 3,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 4,
                    visibility: wgpu::ShaderStages::VERTEX,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Uniform,
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
                /*wgpu::BindGroupLayoutEntry {
                    binding: 5,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        sample_type: wgpu::TextureSampleType::Depth,
                        view_dimension: wgpu::TextureViewDimension::D2,
                        multisampled: false,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 6,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Comparison),
                    count: None,
                },*/
            ],
            label: Some("mesh_renderer_bind_group_layout"),
        });

        let camera_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let light_mat_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let camera_pos_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[f32; 4]>() as u64,
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

        let light_storage = LightStorage::new(&device);
        /*let shadow_renderer = ShadowRenderer::new(
            &light_mat_buffer,
            &model_buffer,
            device,
            surface_caps,
            aligned_model_matrix_size_offset,
        );*/

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
                wgpu::BindGroupEntry {
                    binding: 2,
                    resource: light_storage.get_buffer().as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 3,
                    resource: camera_pos_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 4,
                    resource: light_mat_buffer.as_entire_binding(),
                },
                /*wgpu::BindGroupEntry {
                    binding: 5,
                    resource: wgpu::BindingResource::TextureView(
                        &shadow_renderer.depth_texture.view,
                    ),
                },
                wgpu::BindGroupEntry {
                    binding: 6,
                    resource: wgpu::BindingResource::Sampler(
                        &shadow_renderer.depth_texture.sampler,
                    ),
                },*/
            ],
            label: Some("mesh_renderer_bind_group"),
        });

        let unlit_shader_source = std::fs::read_to_string(UNLIT_SHADER_PATH)
            .expect(&format!("Shader path '{}' was invalid!", UNLIT_SHADER_PATH));

        let unlit_shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(UNLIT_SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(unlit_shader_source.into()),
        });

        let lit_shader_source = std::fs::read_to_string(LIT_SHADER_PATH)
            .expect(&format!("Shader path '{}' was invalid!", LIT_SHADER_PATH));

        let lit_shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some(LIT_SHADER_PATH),
            source: wgpu::ShaderSource::Wgsl(lit_shader_source.into()),
        });

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("mesh_renderer_render_pipeline_layout"),
                bind_group_layouts: &[&bind_group_layout],
                immediate_size: 0,
            });
        let unlit_render_pipeline_descriptor = wgpu::RenderPipelineDescriptor {
            label: Some("mesh_renderer_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &unlit_shader,
                entry_point: Some("vs_main"), // 1.
                buffers: &[vertex::layout()], // 2.
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            },
            fragment: Some(wgpu::FragmentState {
                // 3.
                module: &unlit_shader,
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
                cull_mode: Some(wgpu::Face::Back),
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
        let mut lit_render_pipeline_descriptor = unlit_render_pipeline_descriptor.clone();
        lit_render_pipeline_descriptor.vertex.module = &lit_shader;
        lit_render_pipeline_descriptor
            .fragment
            .as_mut()
            .unwrap()
            .module = &lit_shader;

        let unlit_render_pipeline =
            device.create_render_pipeline(&unlit_render_pipeline_descriptor);
        let lit_render_pipeline = device.create_render_pipeline(&lit_render_pipeline_descriptor);

        MeshRenderer {
            //shadow_renderer: shadow_renderer,
            unlit_render_pipeline: unlit_render_pipeline,
            lit_render_pipeline: lit_render_pipeline,
            bind_group: bind_group,
            camera_buffer: camera_buffer,
            camera_pos_buffer: camera_pos_buffer,
            model_buffer: model_buffer,
            //mesh_scene: MeshScene::new(aligned_model_matrix_size_offset),
            //light_storage: light_storage,
            light_mat_buffer: light_mat_buffer,
            lit_vertex_meshes: Vec::new(),
            lit_index_meshes: Vec::new(),
            unlit_vertex_meshes: Vec::new(),
            unlit_index_meshes: Vec::new(),
            model_byte_offset: aligned_model_matrix_size_offset,
            queue: queue.clone(),
            device: device.clone(),
        }
    }
    /*pub fn create_vertex_mesh(&mut self, descriptor: &VertexMeshDescriptor, device: &wgpu::Device) {
        match descriptor.is_lit {
            true => {
                self.mesh_scene.lit_vertex_meshes.push((
                    VertexMesh::new(&descriptor.vertices, descriptor.num_instances, &device),
                    descriptor.transform_id,
                ));
            }
            false => {
                self.mesh_scene.unlit_vertex_meshes.push((
                    VertexMesh::new(&descriptor.vertices, descriptor.num_instances, &device),
                    descriptor.transform_id,
                ));
            }
        }
    }
    pub fn create_index_mesh(&mut self, descriptor: &IndexMeshDescriptor, device: &wgpu::Device) {
        match descriptor.is_lit {
            true => {
                self.mesh_scene.lit_index_meshes.push((
                    IndexMesh::new(
                        &descriptor.vertices,
                        &descriptor.indices,
                        descriptor.num_instances,
                        &device,
                    ),
                    descriptor.transform_id,
                ));
            }
            false => {
                self.mesh_scene.unlit_index_meshes.push((
                    IndexMesh::new(
                        &descriptor.vertices,
                        &descriptor.indices,
                        descriptor.num_instances,
                        &device,
                    ),
                    descriptor.transform_id,
                ));
            }
        }
    }
    pub fn create_transform(&mut self, transform: &Transform) -> TransformID {
        self.mesh_scene.transforms.push(*transform);
        self.mesh_scene.transforms.len() - 1
    }
    pub fn get_transform(&mut self, transform_id: TransformID) -> Option<&mut Transform> {
        self.mesh_scene.transforms.get_mut(transform_id)
    }*/
    pub(super) fn prepare(&mut self, camera: &Camera, encoder: &mut wgpu::CommandEncoder) {
        let pos = camera.transform.position;
        self.queue.write_buffer(
            &self.camera_pos_buffer,
            0,
            bytemuck::bytes_of(&[pos[0], pos[1], pos[2], 0.0]),
        );

        //self.light_storage.update_buffer(&queue);

        // let transform = TransformNoScale {
        //     position: self.light_storage.point_lights[0].position.into(),
        //     orientation: (-0.3536, 0.3536, 0.1464, 0.8536).into(),
        // };
        // let transform_mat: cgmath::Matrix4<f32> = (&transform).into();
        // let proj = cgmath::ortho(-20.0, 20.0, -20.0, 20.0, 0.1, 100.0);
        // let matrix = proj * transform_mat.invert().unwrap();
        // let matrix_v: [[f32; 4]; 4] = matrix.into();
        // //let matrix: [[f32; 4]; 4] = (&transform).into();
        // queue.write_buffer(&self.light_mat_buffer, 0, bytemuck::bytes_of(&matrix_v));

        let v = camera.get_transformation_matrix().unwrap();
        self.queue
            .write_buffer(&self.camera_buffer, 0, bytemuck::bytes_of(&v));

        //self.shadow_renderer
        //   .build_shadow_pass(&self.mesh_scene, encoder);
    }
    pub(super) fn render(&mut self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_pipeline(&self.unlit_render_pipeline);

        for (i, mesh) in self.unlit_vertex_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_byte_offset) as u32],
            );

            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }
        for (i, mesh) in self.unlit_index_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_byte_offset) as u32],
            );
            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }

        render_pass.set_pipeline(&self.lit_render_pipeline);

        for (i, mesh) in self.lit_vertex_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_byte_offset) as u32],
            );
            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }
        for (i, mesh) in self.lit_index_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_byte_offset) as u32],
            );
            mesh.bind(&mut *render_pass);
            mesh.draw(&mut *render_pass);
        }
    }
}

impl TransformOperationListener for MeshRenderer {
    fn on_insert(&mut self, transform: &Transform, id: ObjectID) {
        let t: [[f32; 4]; 4] = transform.into();
        self.queue.write_buffer(
            &self.model_buffer,
            id as u64 * self.model_byte_offset,
            bytemuck::bytes_of(&t),
        );
        info!("Insert Transform");
    }
    fn on_mod(&mut self, transform: &Transform, id: ObjectID) {
        let t: [[f32; 4]; 4] = transform.into();
        self.queue.write_buffer(
            &self.model_buffer,
            id as u64 * self.model_byte_offset,
            bytemuck::bytes_of(&t),
        );
        info!("Modify Transform");
    }
    fn on_drop(&mut self, _id: ObjectID) {}
}
impl MeshOperationListener for MeshRenderer {
    fn on_mod_index_mesh(&mut self, mesh: &IndexMesh, id: ObjectID) {
        if mesh.is_lit {
            *self.lit_index_meshes.get_mut(id).unwrap() = BakedIndexMesh::new(
                &mesh.vertices,
                &mesh.indices,
                mesh.num_instances,
                &self.device,
            )
        } else {
            *self.unlit_index_meshes.get_mut(id).unwrap() = BakedIndexMesh::new(
                &mesh.vertices,
                &mesh.indices,
                mesh.num_instances,
                &self.device,
            )
        }
    }
    fn on_mod_vertex_mesh(&mut self, mesh: &VertexMesh, id: ObjectID) {
        if mesh.is_lit {
            *self.lit_vertex_meshes.get_mut(id).unwrap() =
                BakedVertexMesh::new(&mesh.vertices, mesh.num_instances, &self.device);
        } else {
            *self.unlit_vertex_meshes.get_mut(id).unwrap() =
                BakedVertexMesh::new(&mesh.vertices, mesh.num_instances, &self.device);
        }
    }
    fn on_insert_index_mesh(&mut self, mesh: &IndexMesh, id: ObjectID) {
        if mesh.is_lit {
            self.lit_index_meshes.push(BakedIndexMesh::new(
                &mesh.vertices,
                &mesh.indices,
                mesh.num_instances,
                &self.device,
            ));
        } else {
            if self.unlit_index_meshes.len() <= id {
                self.unlit_index_meshes.push(BakedIndexMesh::new(
                    &mesh.vertices,
                    &mesh.indices,
                    mesh.num_instances,
                    &self.device,
                ));
            }
        }
    }
    fn on_insert_vertex_mesh(&mut self, mesh: &VertexMesh, id: ObjectID) {
        if mesh.is_lit {
            self.lit_vertex_meshes.push(BakedVertexMesh::new(
                &mesh.vertices,
                mesh.num_instances,
                &self.device,
            ));
        } else {
            if self.unlit_vertex_meshes.len() <= id {
                self.unlit_vertex_meshes.push(BakedVertexMesh::new(
                    &mesh.vertices,
                    mesh.num_instances,
                    &self.device,
                ));
            }
        }
    }
    fn on_drop_index_mesh(&mut self, _id: ObjectID) {}
    fn on_drop_vertex_mesh(&mut self, _id: ObjectID) {}
}
impl LightOperationListener for MeshRenderer {
    fn on_drop_point_light(&mut self, id: ObjectID) {}
    fn on_insert_point_light(&mut self, light: &PointLight, id: ObjectID) {}
    fn on_mod_point_light(&mut self, light: &PointLight, id: ObjectID) {}
}
