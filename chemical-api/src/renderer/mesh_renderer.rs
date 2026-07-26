mod geometry;
mod light_renderer;
mod renderable;
mod rendered_mesh;

use std::{collections::HashMap, num::NonZeroU32};

use geometry::vertex;
use light_renderer::LightRenderer;
use renderable::Renderable;
use rendered_mesh::RenderedMesh;

use crate::{
    Camera,
    renderer::{Renderer, Texture},
    scene::types::{StaticMesh, Transform},
};

#[derive(Debug)]
pub struct MeshRenderer {
    lit_render_pipeline: wgpu::RenderPipeline,
    unlit_render_pipeline: wgpu::RenderPipeline,
    bind_group: wgpu::BindGroup,
    directional_light_map_bind_group: wgpu::BindGroup,
    camera_matrix_buffer: wgpu::Buffer,
    camera_pos_buffer: wgpu::Buffer,

    model_matrix_normal_matrix_buffer: wgpu::Buffer,

    lit_meshes: Vec<RenderedMesh>,
    lit_meshes_sparse: HashMap<hecs::Entity, usize>,
    unlit_meshes: Vec<RenderedMesh>,
    unlit_meshes_sparse: HashMap<hecs::Entity, usize>,

    light_renderer: LightRenderer,

    queue: wgpu::Queue,
    device: wgpu::Device,

    model_matrix_normal_matrix_element_bytesize: u64,
}

const UNLIT_SHADER_PATH: &str = "res/shaders/unlit_shader.wgsl";
const LIT_SHADER_PATH: &str = "res/shaders/lit_shader.wgsl";
const MAX_TRANSFORMS: u64 = 1000;

impl MeshRenderer {
    pub(super) fn new(
        device: &wgpu::Device,
        render_texture_format: wgpu::TextureFormat,
        queue: &wgpu::Queue,
    ) -> Self {
        let bind_group_layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    // Camera matrix
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
                    // Model matrix
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
                    // Directional lights
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
                    // Camera position
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
                    // Directional light count
                    binding: 4,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Buffer {
                        ty: wgpu::BufferBindingType::Storage { read_only: true },
                        has_dynamic_offset: false,
                        min_binding_size: None,
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    // Lightmap sampler
                    binding: 5,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Comparison),
                    count: None,
                },
            ],
            label: Some("mesh_renderer_bind_group_layout"),
        });
        let directional_light_map_bind_group_layout =
            device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
                entries: &[wgpu::BindGroupLayoutEntry {
                    // Directional light maps
                    binding: 0,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        sample_type: wgpu::TextureSampleType::Depth,
                        view_dimension: wgpu::TextureViewDimension::D2,
                        multisampled: false,
                    },
                    count: NonZeroU32::new(LightRenderer::MAX_DIRECTIONAL_LIGHTS as u32),
                }],
                label: Some("mesh_renderer_directional_light_map_bind_group_layout"),
            });

        let camera_matrix_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[[f32; 4]; 4]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });
        let camera_pos_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: size_of::<[f32; 3]>() as u64,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let matrix_offset_uniform_alignment: u64 = wgpu::util::align_to(
            std::mem::size_of::<[[f32; 4]; 4]>() as u32,
            device.limits().min_uniform_buffer_offset_alignment,
        ) as u64;
        let model_matrix_normal_matrix_element_bytesize: u64 = matrix_offset_uniform_alignment
            + wgpu::util::align_to(
                std::mem::size_of::<[[f32; 3]; 3]>() as u32,
                device.limits().min_uniform_buffer_offset_alignment,
            ) as u64;
        let model_matrix_normal_matrix_buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label: None,
            size: model_matrix_normal_matrix_element_bytesize * MAX_TRANSFORMS,
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
            mapped_at_creation: false,
        });

        let light_renderer = LightRenderer::new(&bind_group_layout, device, queue);

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[
                wgpu::BindGroupEntry {
                    // Camera matrix
                    binding: 0,
                    resource: camera_matrix_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    // Model matrix
                    binding: 1,
                    resource: wgpu::BindingResource::Buffer(wgpu::BufferBinding {
                        buffer: &model_matrix_normal_matrix_buffer,
                        offset: 0,
                        size: wgpu::BufferSize::new(model_matrix_normal_matrix_element_bytesize),
                    }),
                },
                wgpu::BindGroupEntry {
                    // Directional lights
                    binding: 2,
                    resource: light_renderer
                        .get_directional_lights_buffer()
                        .as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    // Camera pos
                    binding: 3,
                    resource: camera_pos_buffer.as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    // Directional light count
                    binding: 4,
                    resource: light_renderer
                        .get_directional_light_count_buffer()
                        .as_entire_binding(),
                },
                wgpu::BindGroupEntry {
                    binding: 5,
                    resource: wgpu::BindingResource::Sampler(
                        light_renderer.shadow_renderer.get_lightmap_sampler(),
                    ),
                },
            ],
            label: Some("mesh_renderer_bind_group"),
        });
        let directional_light_map_bind_group =
            device.create_bind_group(&wgpu::BindGroupDescriptor {
                layout: &directional_light_map_bind_group_layout,
                entries: &[wgpu::BindGroupEntry {
                    binding: 0,
                    resource: wgpu::BindingResource::TextureViewArray(
                        &light_renderer.shadow_renderer.get_directional_lightmaps(),
                    ),
                }],
                label: Some("mesh_renderer_directional_light_map_bind_group"),
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
                bind_group_layouts: &[&bind_group_layout, &directional_light_map_bind_group_layout],
                push_constant_ranges: &[],
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
                    format: render_texture_format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
                compilation_options: wgpu::PipelineCompilationOptions::default(),
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList, // 1.
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw, // 2.
                cull_mode: None,                  //Some(wgpu::Face::Back),
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
            cache: None, // 6.
            multiview: None,
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
            lit_render_pipeline,
            unlit_render_pipeline,
            bind_group,
            model_matrix_normal_matrix_element_bytesize,
            model_matrix_normal_matrix_buffer,
            camera_pos_buffer,
            camera_matrix_buffer,
            light_renderer,
            lit_meshes_sparse: HashMap::new(),
            unlit_meshes_sparse: HashMap::new(),
            lit_meshes: Vec::new(),
            unlit_meshes: Vec::new(),
            queue: queue.clone(),
            device: device.clone(),
            directional_light_map_bind_group,
        }
    }
    pub fn prepare(
        &mut self,
        camera: &Camera,
        world: &hecs::World,
        encoder: &mut wgpu::CommandEncoder,
    ) {
        let mut lit_mesh_idx = 0_u32;
        let mut unlit_mesh_idx = 0_u32;
        for (entity, static_mesh, transform) in world
            .query::<(hecs::Entity, &StaticMesh, &Transform)>()
            .iter()
        {
            if static_mesh.is_lit {
                if !self.lit_meshes_sparse.contains_key(&entity) {
                    self.lit_meshes_sparse.insert(entity, self.lit_meshes.len());
                    self.lit_meshes.push(RenderedMesh::new(
                        &static_mesh.vertices,
                        static_mesh.indices.as_ref().map(|v| &**v),
                        &self.device,
                    ));
                }
                let model_matrix = transform.to_matrix();
                self.queue.write_buffer(
                    &self.model_matrix_normal_matrix_buffer,
                    unlit_mesh_idx as u64 * self.model_matrix_normal_matrix_element_bytesize,
                    bytemuck::bytes_of(&model_matrix),
                );
                let normal_matrix = glam::Mat3::from_mat4(model_matrix).inverse().transpose();

                self.queue.write_buffer(
                    &self.model_matrix_normal_matrix_buffer,
                    size_of::<[[f32; 4]; 4]>() as u64
                        + (unlit_mesh_idx as u64
                            * self.model_matrix_normal_matrix_element_bytesize),
                    bytemuck::bytes_of(&normal_matrix),
                );
                unlit_mesh_idx += 1;
            } else if !static_mesh.is_lit {
                if !self.unlit_meshes_sparse.contains_key(&entity) {
                    self.unlit_meshes_sparse
                        .insert(entity, self.unlit_meshes.len());
                    self.unlit_meshes.push(RenderedMesh::new(
                        &static_mesh.vertices,
                        static_mesh.indices.as_ref().map(|v| &**v),
                        &self.device,
                    ));
                }
                let model_matrix = transform.to_matrix();
                self.queue.write_buffer(
                    &self.model_matrix_normal_matrix_buffer,
                    lit_mesh_idx as u64 * self.model_matrix_normal_matrix_element_bytesize,
                    bytemuck::bytes_of(&model_matrix),
                );
                let normal_matrix = glam::Mat3::from_mat4(model_matrix).inverse().transpose();

                self.queue.write_buffer(
                    &self.model_matrix_normal_matrix_buffer,
                    size_of::<[[f32; 4]; 4]>() as u64
                        + (lit_mesh_idx as u64 * self.model_matrix_normal_matrix_element_bytesize),
                    bytemuck::bytes_of(&normal_matrix),
                );
                lit_mesh_idx += 1;
            }
        }
        self.light_renderer.prepare(world);

        let pos = camera.transform.position;
        let pos_into: [f32; 3] = pos.into();
        self.queue
            .write_buffer(&self.camera_pos_buffer, 0, bytemuck::bytes_of(&pos_into));

        let v = camera.get_camera_matrix();
        self.queue
            .write_buffer(&self.camera_matrix_buffer, 0, bytemuck::bytes_of(&v));

        loop {
            let render_pass = self
                .light_renderer
                .start_queued_directional_light_shadow_pass(encoder);

            match render_pass {
                Some(mut rp) => {
                    for (i, mesh) in self.lit_meshes.iter().enumerate() {
                        rp.set_bind_group(
                            0,
                            &self.bind_group,
                            &[
                                (i as u64 * self.model_matrix_normal_matrix_element_bytesize)
                                    as u32,
                            ],
                        );
                        mesh.bind(&mut rp);
                        mesh.draw(&mut rp);
                    }
                }
                None => break,
            }
        }
    }
    pub fn render(&self, render_pass: &mut wgpu::RenderPass) {
        render_pass.set_pipeline(&self.unlit_render_pipeline);
        render_pass.set_bind_group(1, &self.directional_light_map_bind_group, &[]);

        for (i, mesh) in self.unlit_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_matrix_normal_matrix_element_bytesize) as u32],
            );
            mesh.bind(render_pass);
            mesh.draw(render_pass);
        }

        render_pass.set_pipeline(&self.lit_render_pipeline);
        for (i, mesh) in self.lit_meshes.iter().enumerate() {
            render_pass.set_bind_group(
                0,
                &self.bind_group,
                &[(i as u64 * self.model_matrix_normal_matrix_element_bytesize) as u32],
            );
            mesh.bind(render_pass);
            mesh.draw(render_pass);
        }
    }
}
