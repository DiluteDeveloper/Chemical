
// Vertex shader

struct ModelNormalMatrix {
    model_matrix: mat4x4<f32>,
    normal_matrix: mat3x3<f32>}
// Group is the order specified in the render pipeline layout descriptor
@group(0) @binding(0) 
var<uniform> camera_matrix: mat4x4<f32>;
@group(0) @binding(1) 
var<uniform> model_normal_matrix: ModelNormalMatrix;

struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) normal: vec3<f32>,
}

struct VertexOutput {
    @builtin(position) position: vec4<f32>,
    @location(0) normal: vec3<f32>,
    @location(1) world_position: vec3<f32>,
}

@vertex
fn vs_main(
    vtx: VertexInput,
    @builtin(instance_index) idx: u32
) -> VertexOutput {
    var out: VertexOutput;
    out.position = camera_matrix * model_normal_matrix.model_matrix * vec4<f32>(vtx.position, 1.0);
    // Take note that normals are not transformed to the world position; i.e, if you rotate the model the normals will be wrong!
    out.normal = normalize(model_normal_matrix.normal_matrix * vtx.normal);
    out.world_position = vec3<f32>((model_normal_matrix.model_matrix * vec4<f32>(vtx.position, 1.0)).xyz);
    return out;
}
// Fragment shader

// Alignment of 16 bytes, size of 80 bytes
// 16*5=80 bytes, so fits neatly into array
struct DirectionalLight {
    colour: vec3<f32>,
    strength: f32,
    projection: mat4x4<f32>}

@group(0) @binding(2)
var<storage> directional_lights: array<DirectionalLight>;
@group(0) @binding(4)
var<storage> directional_light_count: u32; 

@group(1) @binding(0) var directional_light_maps: binding_array<texture_depth_2d, 256>;
@group(0) @binding(5) var depth_sampler: sampler_comparison;

@group(0) @binding(3) 
var<uniform> camera_position: vec3<f32>;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4<f32>(process_directional_lights(in.normal, in.world_position).xyz, 1.0);
}

const DIFFUSE_STRENGTH: f32 = 1.0;
const SPECULAR_STRENGTH: f32 = 0.01;

fn process_directional_lights(normal: vec3<f32>, world_position: vec3<f32>) -> vec3<f32> {

    var test_pos = vec3f(20.0, 20.0, 20.0);
    var surface_colour = vec3f(0);
    let view_dir = normalize(camera_position - world_position);

    for (var i = 0u; i < directional_light_count; i++) {

        let fragment_to_light_vector = test_pos - world_position;
        let dist = length(fragment_to_light_vector);
        let dir = normalize(fragment_to_light_vector);

        let radiance = directional_lights[i].colour * (1 / pow(dist, 2));
        let nDotL = max(dot(normal, dir), 0);
        let diff = radiance * nDotL;

        let reflect_dir = reflect(-(dir.xyz), normal.xyz);

        let spec = directional_lights[i].colour * pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);

        let amb = directional_lights[i].colour;

        let mapped_position = directional_lights[i].projection * vec4<f32>(world_position, 1.0);
        let shadow_coord = mapped_position.xyz / mapped_position.w;
        let uv = shadow_coord.xy * vec2(0.5, -0.5) + 0.5;
        let shadow = textureSampleCompare(directional_light_maps[i], depth_sampler, uv, mapped_position.z);
        surface_colour += (((spec * SPECULAR_STRENGTH) + (diff * DIFFUSE_STRENGTH)) * directional_lights[i].strength) * shadow;
    }
    return surface_colour;
}
