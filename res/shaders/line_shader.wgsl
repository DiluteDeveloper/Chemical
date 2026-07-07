// Vertex shader

const OPENGL_TO_WGPU_MATRIX: mat4x4<f32> = mat4x4<f32>(
    vec4<f32>(1.0, 0.0, 0.0, 0.0),
    vec4<f32>(0.0, 1.0, 0.0, 0.0),
    vec4<f32>(0.0, 0.0, 0.5, 0.0),
    vec4<f32>(0.0, 0.0, 0.5, 1.0),
);
// Group is the order specified in the render pipeline layout descriptor
@group(0) @binding(0) 
var<uniform> camera_matrix: mat4x4<f32>;
@group(0) @binding(1) 
var<uniform> camera_position: vec4<f32>;

struct VertexInput {
	@location(0) p1_position: vec3<f32>,
	@location(1) p1p0_normal: vec3<f32>,
	@location(2) p2_position: vec3<f32>,
	@location(3) p2p1_normal: vec3<f32>,
}

struct VertexOutput {
  @builtin(position) position: vec4<f32>,
  @location(0) instance_idx: u32
}

@vertex
fn vs_main(
	vtx: VertexInput,
	@builtin(vertex_index) idx: u32,
  @builtin(instance_index) i_idx: u32
) -> VertexOutput {
    var out: VertexOutput;
    var p1_cross = cross(normalize(vtx.p1_position - camera_position.xyz), vtx.p1p0_normal);
    var p2_cross = cross(normalize(vtx.p2_position - camera_position.xyz), vtx.p2p1_normal);
    var fpos : vec3<f32>;
    switch(idx % 6) {
      case 0: {
        fpos = vtx.p1_position + (p1_cross * 40);
      }
      case 1: {
        fpos = vtx.p1_position - (p1_cross * 40);
      }
      case 2: {
        fpos = vtx.p2_position - (p2_cross * 40);
      }
      case 3: {
        fpos = vtx.p1_position + (p1_cross * 40);
      }
      case 4: {
        fpos = vtx.p2_position - (p2_cross * 40);
      }
      case 5: {
        fpos = vtx.p2_position + (p2_cross * 40);
      }
      default: {
        fpos = vec3<f32>(500.0, 0.0, 0.0);
      }
    }
    out.position = OPENGL_TO_WGPU_MATRIX * camera_matrix * vec4<f32>(fpos, 1.0);
    out.instance_idx = i_idx;
    //out.position = vec4<f32>(fpos, 1.0);

    return out;
}
// Fragment shader

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	return vec4<f32>(f32(in.instance_idx % 100) / 100.0, f32(in.instance_idx % 1000) / 1000.0, f32(in.instance_idx % 10000) / 10000.0, 1.0);
}
