// Vertex shader
struct CameraUniform {
    view_proj: mat4x4<f32>,
};
// Group is the order specified in the render pipeline layout descriptor
@group(1) @binding(0) 
var<uniform> camera: CameraUniform;

struct VertexInput {
	@location(0) position: vec3<f32>,
	@location(1) tex_coords: vec2<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
		@location(0) tex_coords: vec2<f32>,
}

@vertex
fn vs_main(
	in: VertexInput
) -> VertexOutput {
    var out: VertexOutput;
    out.position = camera.view_proj * vec4<f32>(in.position, 1.0);
		out.tex_coords = in.tex_coords;
    return out;
}
// Fragment shader

@group(0) @binding(0)
var t_diffuse: texture_2d<f32>;
@group(0) @binding(1)
var s_diffuse: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
	return textureSample(t_diffuse, s_diffuse, in.tex_coords);
  //return vec4<f32>(in.colour, 1.0);
}
