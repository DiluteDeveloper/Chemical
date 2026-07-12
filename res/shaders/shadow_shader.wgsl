// Vertex shader

struct ModelNormalMatrix {
    model_matrix: mat4x4<f32>,
    normal_matrix: mat3x3<f32>}
// Group is the order specified in the render pipeline layout descriptor
@group(0) @binding(1) 
var<uniform> model_normal_matrix: ModelNormalMatrix;
@group(1) @binding(0)
var<uniform> projection: mat4x4<f32>;

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
    out.position = projection * model_normal_matrix.model_matrix * vec4<f32>(vtx.position, 1.0);
    return out;
}