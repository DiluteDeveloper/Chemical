// Vertex Shader

struct VertexOutput {
    @builtin(position)
    position: vec4<f32>,
    @location(0)
    uv: vec2<f32>,
}

@vertex
fn vs_main(
    @builtin(vertex_index)idx: u32
) -> VertexOutput {
    var out: VertexOutput;
    var uv = vec2<u32>((idx << 1) & 2, idx & 2);
    out.position = vec4(f32(uv.x - ((uv.x ^ 2) >> 1)), f32(uv.y - ((uv.y ^ 2) >> 1)) * -1.0f, 0.0f, 1.0f);
    out.uv = vec2<f32>(uv);
    return out;
}

// Fragment Shader

@group(0) @binding(0) var framebuffer_sampler: sampler;
@group(0) @binding(1) var framebuffer_texture: texture_2d<f32>;
@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return textureSample(framebuffer_texture, framebuffer_sampler, vec2<f32>(in.uv));
}
