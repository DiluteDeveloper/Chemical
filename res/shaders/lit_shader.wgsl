
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
var<uniform> model_matrix: mat4x4<f32>;
@group(0) @binding(4) 
var<uniform> light_matrix: mat4x4<f32>;

struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) normal: vec3<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) normal: vec3<f32>,
    @location(1) world_position: vec3<f32>,
    @location(2) position_on_lightmap: vec4<f32>}

@vertex
fn vs_main(
    vtx: VertexInput,
    @builtin(instance_index) idx: u32
) -> VertexOutput {
    var out: VertexOutput;
    out.position = OPENGL_TO_WGPU_MATRIX * camera_matrix * model_matrix * vec4<f32>(vtx.position, 1.0);
    out.position_on_lightmap = OPENGL_TO_WGPU_MATRIX * light_matrix * model_matrix * vec4<f32>(vtx.position, 1.0);
    // Take note that normals are not transformed to the world position; i.e, if you rotate the model the normals will be wrong!
    out.normal = vtx.normal;
    out.world_position = vec3<f32>((model_matrix * vec4<f32>(vtx.position, 1.0)).xyz);
    return out;
}
// Fragment shader

struct PointLight {
    position: vec3<f32>,
    strength: f32,
    colour: vec3<f32>}
    // 4 bytes of padding

struct LightStorage {
    pointCount: u32,
    // 12 bytes of padding
    point_lights: array<PointLight>,
}

/*@group(0) @binding(0)
var t_diffuse: texture_2d<f32>;
@group(0) @binding(1)
var s_diffuse: sampler;*/
@group(0) @binding(2)
var<storage> lights: LightStorage;
@group(0) @binding(3) 
var<uniform> camera_position: vec4<f32>;
@group(0) @binding(5) var light_texture: texture_depth_2d;
@group(0) @binding(6) var light_sampler: sampler_comparison;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(process_point_lights(in.normal, in.world_position, in.position_on_lightmap).xyz, 1.0);
    //return vec4<f32>(in.colour, 1.0);
}

const DIFFUSE_STRENGTH: f32 = 1.0;
const SPECULAR_STRENGTH: f32 = 0.01;

fn process_point_lights(normal: vec3<f32>, world_position: vec3<f32>, position_on_lightmap: vec4<f32>) -> vec3<f32> {
    var surface_colour = vec3f(0);
    let view_dir = normalize(camera_position.xyz - world_position);
    for (var i = 0u; i < lights.pointCount; i++) {

        let fragment_to_light_vector = lights.point_lights[i].position - world_position;
        let dist = length(fragment_to_light_vector);
        let dir = normalize(fragment_to_light_vector);

        let radiance = lights.point_lights[i].colour * (1 / pow(dist, 2));
        let nDotL = max(dot(normal, dir), 0);
        let diff = radiance * nDotL;

        let reflect_dir = reflect(-(dir.xyz), normal.xyz);

        let spec = lights.point_lights[i].colour * pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);

        let amb = lights.point_lights[i].colour;

        let shadow_coord = position_on_lightmap.xyz / position_on_lightmap.w;
        let uv = shadow_coord.xy * vec2(0.5, -0.5) + 0.5;
        let shadow = textureSampleCompare(light_texture, light_sampler, uv, position_on_lightmap.z);
        surface_colour += (((spec * SPECULAR_STRENGTH) + (diff * DIFFUSE_STRENGTH)) * lights.point_lights[i].strength) * shadow;
    }
    return surface_colour;
}