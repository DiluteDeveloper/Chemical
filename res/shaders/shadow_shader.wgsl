// Vertex shader

const OPENGL_TO_WGPU_MATRIX: mat4x4<f32> = mat4x4<f32>(
    vec4<f32>(1.0, 0.0, 0.0, 0.0),
    vec4<f32>(0.0, 1.0, 0.0, 0.0),
    vec4<f32>(0.0, 0.0, 0.5, 0.0),
    vec4<f32>(0.0, 0.0, 0.5, 1.0),
);
struct ModelNormalMatrix {
    model_matrix: mat4x4<f32>,
    normal_matrix: mat3x3<f32>}
// Group is the order specified in the render pipeline layout descriptor
@group(0) @binding(1) 
var<uniform> model_normal_matrix: ModelNormalMatrix;

// Alignment of 16 bytes, size of 80 bytes
// 16*5=80 bytes, so fits neatly into array
struct DirectionalLight {
    colour: vec3<f32>,
    strength: f32,
    projection: mat4x4<f32>}

@group(1) @binding(0)
var<storage> directional_light: DirectionalLight;

struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) normal: vec3<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
}

@vertex
fn vs_main(
    vtx: VertexInput,
) -> VertexOutput {
    var out: VertexOutput;
    out.position = OPENGL_TO_WGPU_MATRIX * directional_light.projection * model_normal_matrix.model_matrix * vec4<f32>(vtx.position, 1.0);
    return out;
}