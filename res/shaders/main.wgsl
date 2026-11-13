
struct VertexInput {
    @location(0) vertex_position: vec3f, 
    @location(1) colour: vec3f
}
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) colour: vec3f,
}

@group(0) @binding(0) var<uniform> view: mat4x4<f32>;
@group(0) @binding(1) var<uniform> proj: mat4x4<f32>;
@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = proj * view * vec4f(in.vertex_position.xyz, 1.0);
    out.colour = in.colour;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    return vec4f(in.colour, 1.0);
}
