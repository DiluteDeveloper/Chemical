struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) @interpolate(flat) orientation: u32,
}

@group(0) @binding(0) var<uniform> view: mat4x4<f32>;
@group(0) @binding(1) var<uniform> proj: mat4x4<f32>;

struct BlockFace {
    position: vec4i,
    misc: vec4u,
};
@group(1) @binding(0) var<storage, read> faces: array<BlockFace>;


@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    var out: VertexOutput;

    let face_idx = idx / 6;
    let face = faces[face_idx];
    let face_start_pos = vec3f(face.position.xyz);
    let face_orientation = face.misc.x;
    let second_vertex_offset_v = face.misc.y;
    let third_vertex_offset_v = face.misc.z;

//	const start_pos_first_offset_mask = vec4u(0x0, 0x0, 0x0, 0xFFFF0000);
//	const start_pos_last_offset_mask = vec4u(0x0, 0x0, 0x0, 0x0000FFFF);
//	//const misc_first_offset_bitshift_mask = vec4u(0xFF000000, 0x0, 0x0, 0x0);
//	//const misc_last_offset_bitshift_mask = vec4u(0x00FF0000, 0x0, 0x0, 0x0);FF000000

//	let start_pos_array = array<u32, 4>(start_pos.x,start_pos.y,start_pos.z,start_pos.w);
//	let start_pos_first_offset = (start_pos_array & start_pos_first_offset_mask) << (faces[face_idx].misc.x >> 6);
//	let start_pos_last_offset = (array<u32, 4>(start_pos.x) & start_pos_last_offset_mask) << ((faces[face_idx].misc.x >> 4) & 0xFF);

//	let second_tri_pos = start_pos + vec4i(start_pos_first_offset);
//	let last_tri_pos = start_pos + vec4i(start_pos_last_offset);
//	let end_pos = start_pos + vec4i(start_pos_first_offset) + vec4i(start_pos_last_offset);

    const FACE_TABLE = array<vec3u, 12>(
        vec3u(1, 0, 0), // Second top vertex offset mask
        vec3u(0, 0, 1), // Third top vertex offset mask
        vec3u(0, 0, 1), // Second bottom vertex offset mask
        vec3u(1, 0, 0), // Third bottom vertex offset mask

        vec3u(0, 1, 0), // Second left vertex offset mask
        vec3u(0, 0, 1), // Third left vertex offset mask
        vec3u(0, 0, 1), // Second right vertex offset mask
        vec3u(0, 1, 0), // Third right vertex offset mask

        vec3u(0, 1, 0), // Second front vertex offset mask
        vec3u(1, 0, 0), // Third front verte offset mask
        vec3u(1, 0, 0), // Second back vertex offset mask
        vec3u(0, 1, 0), // Third back vertex offset mask


    );

    let second_vertex_offset = (FACE_TABLE[face_orientation * 2] * second_vertex_offset_v);
    let second_vertex_pos = face_start_pos + vec3f(second_vertex_offset);

    let third_vertex_offset = (FACE_TABLE[(face_orientation * 2) + 1] * third_vertex_offset_v);
    let third_vertex_pos = face_start_pos + vec3f(third_vertex_offset);

    let last_vertex_pos = face_start_pos + vec3f(second_vertex_offset) + vec3f(third_vertex_offset);

    switch (idx % 6) {
        case 0: {
            out.position = proj * view * vec4f(face_start_pos, 1);
            break;
        }
        case 1, 3: {
            out.position = proj * view * vec4f(second_vertex_pos, 1);
            break;
        }
        case 2, 5: {
            out.position = proj * view * vec4f(third_vertex_pos, 1);
            break;
        }
        case 4: {
            out.position = proj * view * vec4f(last_vertex_pos, 1);
            break;
        }
        default: { break; }
    }
    out.orientation = face_orientation;
    return out;
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {
    const NORMAL_TABLE = array<vec3f, 6>(
        vec3f(0, 1, 0), // Second top vertex offset mask
        vec3f(0, -1, 0), // Third bottom vertex offset mask
        vec3f(-1, 0, 0), // Third left vertex offset mask
        vec3f(1, 0, 0), // Third right vertex offset mask
        vec3f(0, 0, 1), // Third front verte offset mask
        vec3f(0, 0, -1), // Third back vertex offset mask

    );
    return vec4f(NORMAL_TABLE[in.orientation], 1.0);
}
