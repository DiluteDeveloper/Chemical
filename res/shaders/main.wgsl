struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) @interpolate(flat) orientation: u32,
    @location(1) uv: vec2f,
    @location(2) @interpolate(flat) block_type: u32,
}

@group(0) @binding(0) var<uniform> view: mat4x4<f32>;
@group(0) @binding(1) var<uniform> proj: mat4x4<f32>;

struct BlockFace {
    position: vec4i,
    misc: vec4u,
};
@group(1) @binding(0) var<storage, read> faces: array<BlockFace>;

//fn RotateUVClockwise(uv: vec2u) -> vec2u {
//    return vec2u(1 - uv.y, uv.x);
//}
//fn RotateUVAntiClockwise(uv: vec2u) -> vec2u {
//    return vec2u(uv.y, 1 - uv.x);
//}
//fn GetRotatedUVs(uv: vec2u, orientation: u32) -> vec2u {
//    switch(orientation) {
//        case 0: return uv; // x - (0, 0)
//        case 1: return vec2u(1 - uv.x, 1 - uv.y);// x - (1, 1)
//        case 2: return RotateUVAntiClockwise(uv);
//    }
//}
@vertex
fn vs_main(@builtin(vertex_index) idx: u32) -> VertexOutput {
    var out: VertexOutput;

    let face_idx = idx / 6;
    let face = faces[face_idx];
    let v0_pos = face.position.xyz;
    let orientation = face.misc.x;
    let size = vec2u(face.misc.y, face.misc.z);

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

    const UV_TABLE = array<vec2u, 4*6>(
        vec2u(1, 1), vec2u(0, 1), vec2u(1, 0), vec2u(0, 0),// Top face UV 1, UV 2, UV 3, UV 4
        vec2u(1, 0), vec2u(1, 1), vec2u(0, 0), vec2u(0, 1),// Bottom face UV 1, UV 2, UV 3, UV 4
        vec2u(1, 1), vec2u(0, 1), vec2u(1, 0), vec2u(0, 0),// Right face UV 1, UV 2, UV 3, UV 4
        vec2u(0, 1), vec2u(0, 0), vec2u(1, 1), vec2u(1, 0),// Left face UV 1, UV 2, UV 3, UV 4
        vec2u(1, 1), vec2u(0, 1), vec2u(1, 0), vec2u(0, 0),// Back face UV 1, UV 2, UV 3, UV 4
        vec2u(0, 1), vec2u(0, 0), vec2u(1, 1), vec2u(1, 0),// Front face UV 1, UV 2, UV 3, UV 4
    );

    const FACE_TABLE = array<vec3u, 12>(
        vec3u(1, 0, 0), // Second top vertex offset mask
        vec3u(0, 0, 1), // Third top vertex offset mask
        vec3u(0, 0, 1), // Second bottom vertex offset mask
        vec3u(1, 0, 0), // Third bottom vertex offset mask

        vec3u(0, 0, 1), // Second right vertex offset mask
        vec3u(0, 1, 0), // Third right vertex offset mask
        vec3u(0, 1, 0), // Second left vertex offset mask
        vec3u(0, 0, 1), // Third left vertex offset mask

        vec3u(1, 0, 0), // Second back vertex offset mask
        vec3u(0, 1, 0), // Third back vertex offset mask
        vec3u(0, 1, 0), // Second front vertex offset mask
        vec3u(1, 0, 0), // Third front verte offset mask

    );

    // When the UV table is used to rotate UVs the size may need to be flipped to account
    let sized_uv = select(size.yx, size.xy, orientation % 2 == 0);

    let v1_dim_size = vec3i(FACE_TABLE[orientation * 2] * size.x);
    let v1_pos = v0_pos + v1_dim_size;

    let v2_dim_size = vec3i(FACE_TABLE[(orientation * 2) + 1] * size.y);
    let v2_pos = v0_pos + v2_dim_size;

    let v3_pos = v0_pos + v1_dim_size + v2_dim_size;

    switch (idx % 6) {
        case 0: {
            out.position = proj * view * vec4f(vec3f(v0_pos), 1);
            out.uv = vec2f(UV_TABLE[orientation * 4] * sized_uv);
            //out.uv = vec2f(RotateUVAntiClockwise(vec2u(0, 0)) * vec2u(third_vertex_offset_v, second_vertex_offset_v));
            break;
        }
        case 1, 3: {
            out.position = proj * view * vec4f(vec3f(v1_pos), 1);
            out.uv = vec2f(UV_TABLE[(orientation * 4) + 1] * sized_uv);
            break;
        }
        case 2, 5: {
            out.position = proj * view * vec4f(vec3f(v2_pos), 1);
            out.uv = vec2f(UV_TABLE[(orientation * 4) + 2] * sized_uv);
            break;
        }
        case 4: {
            out.position = proj * view * vec4f(vec3f(v3_pos), 1);
            out.uv = vec2f(UV_TABLE[(orientation * 4) + 3] * sized_uv);
            break;
        }
        default: { break; }
    }
    out.orientation = orientation;
    out.block_type = u32(face.position.w);
    return out;
}

@group(1) @binding(1) var texture: texture_2d_array<f32>;
@group(1) @binding(2) var texture_sampler: sampler;

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
    //return vec4f(NORMAL_TABLE[in.orientation], 1.0);
    return vec4f(textureSample(texture, texture_sampler, in.uv, in.block_type).rgb, 1.0);
}
