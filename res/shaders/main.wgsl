struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) colour: vec3f,
}

@group(0) @binding(0) var<uniform> view: mat4x4<f32>;
@group(0) @binding(1) var<uniform> proj: mat4x4<f32>;

// Fourth component is unused but matches alignment
@group(1) @binding(0) var<storage, read> block_positions: array<vec4f>;

@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    var out: VertexOutput;
    let mod_idx = idx % 6;
    let start_pos = vec3f(block_positions[idx / 6].xyz);
    let end_pos = vec3f(block_positions[(idx / 6) + 1].xyz);
    switch (mod_idx) {
        case 0: {
            out.position = proj * view * vec4f(start_pos, 1.0);
            break;
        }
        case 1, 3: {
            out.position = proj * view * vec4f(start_pos.x, end_pos.y, start_pos.z, 1.0);
            break;
        }
        case 2, 5: {
            out.position = proj * view * vec4f(end_pos.x, start_pos.y, end_pos.z, 1.0);
            break;
        }
        case 4: {
            out.position = proj * view * vec4f(end_pos, 1.0);
            break;
        }
        default: { break; }
    }
    out.colour = vec3f(1.0, 1.0, 1.0);
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    return vec4f(in.colour, 1.0);
}
