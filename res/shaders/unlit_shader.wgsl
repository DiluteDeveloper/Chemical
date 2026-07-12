// Vertex shader

// Group is the order specified in the render pipeline layout descriptor
@group(0) @binding(0) 
var<uniform> camera_matrix: mat4x4<f32>;
@group(0) @binding(1) 
var<uniform> model_matrix: mat4x4<f32>;

struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) normal: vec3<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) normal: vec3<f32>,
}

@vertex
fn vs_main(
    vtx: VertexInput,
    @builtin(instance_index) idx: u32
) -> VertexOutput {
    var out: VertexOutput;
    out.position = camera_matrix * model_matrix * vec4<f32>(vtx.position, 1.0);
    out.normal = vtx.normal;
    return out;
}
// Fragment shader

@group(0) @binding(0)
var t_diffuse: texture_2d<f32>;
@group(0) @binding(1)
var s_diffuse: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(in.normal.xyz, 1.0);
    //return vec4<f32>(in.colour, 1.0);
}
