
struct VertexInput {
    @location(0) vertex_position: vec2f, 
    @location(1) colour: vec3f
}
struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) colour: vec3f,
}
@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = vec4f(in.vertex_position, 0.0, 1.0);
    out.colour = in.colour;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    return vec4f(in.colour, 1.0);
}
