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
    var uv = vec2(i32((idx << 1) & 2), i32(idx & 2));
    out.position = vec4((f32(uv.x) - 1.0f) + f32(uv.x & 2),((f32(uv.y) - 1.0f) + f32(uv.y & 2)) * -1.0f, 0.0f, 1.0f);
    out.uv = vec2<f32>(uv);
    return out;
}

// Fragment Shader

@group(0) @binding(0) var framebuffer_sampler: sampler;
@group(0) @binding(1) var framebuffer_texture: texture_2d<f32>;
@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    let gamma = 1.0f;
    let exposure = 4.5f;
    let hdr = textureSample(framebuffer_texture, framebuffer_sampler, in.uv).rgb;
    let hdr2 = textureSample(framebuffer_texture, framebuffer_sampler, vec2(in.uv.x, in.uv.y + (sin(in.uv.x * 20.0f) / 10.0f))).rgb;
    var mapped = vec3(1.0f) - exp(-hdr2 * exposure);
    mapped = pow(mapped, vec3(1.0f / gamma));

    return vec4<f32>(mapped, 1.0f);
}
