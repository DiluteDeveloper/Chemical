use super::{Index, Vertex};
use cgmath::{InnerSpace, Vector3, num_traits::pow};

const PI: f32 = 3.141592;

// Resolution represents how many triangles per horizontal loop and vertical half loop
#[allow(unused)]
pub fn generate_vertex_sphere(resolution: u8) -> anyhow::Result<Vec<Vertex>, String> {
    let sized_res = resolution as u32;
    if sized_res <= 2 {
        return Err("Resolution is less than or equal to 2!".to_string());
    }
    // Recording this to check if it is the right size for the job at the end of the function let vertex_capacity = ((resolution as usize - 1) * resolution as usize) + 2;
    let vertex_capacity = (sized_res * (sized_res - 2) * 6) + sized_res * 6;

    let mut vertices: Vec<Vertex> = Vec::with_capacity(vertex_capacity as usize);

    // Recording this to check if it is the right size for the job at the end
    // of the function
    //let index_capacity = (sized_res * sized_res * 3) as usize;
    //
    //let mut indices: Vec<Index> = Vec::with_capacity(index_capacity);

    // angle of a triangle section of the circle in radians (divide by 2 for
    // half of a circle for vertical)
    let res_radians = 1.0 / (sized_res as f32 / (PI * 2_f32));

    for v_int in 0..sized_res {
        // Iterate through the vertical levels of the icosphere
        for h_int in 0..sized_res {
            // Iterate through each vertex on the current Y plane of the sphere

            let v = v_int as f32;
            let h = h_int as f32;

            let h_offset = v * (res_radians / 2.0);

            let prev_angle = ((h - 1.0) * res_radians) + h_offset;
            let cur_angle = (h * res_radians) + h_offset;
            let next_angle = ((h + 1.0) * res_radians) + h_offset;

            // sized_res in radians is half because vertical is only half of a
            // circle
            let cur_angle_vert = v * (res_radians / 2.0);
            let next_angle_vert = (v + 1.0) * (res_radians / 2.0);

            let v0_pos: Vector3<f32> = Vector3::new(
                cur_angle.cos() * cur_angle_vert.sin(),
                cur_angle_vert.cos(),
                (cur_angle).sin() * (cur_angle_vert).sin(),
            );

            let v1_pos: Vector3<f32> = Vector3::new(
                (next_angle - (res_radians / 2.0)).cos() * (next_angle_vert).sin(),
                (next_angle_vert).cos(),
                (next_angle - (res_radians / 2.0)).sin() * (next_angle_vert).sin(),
            );

            let v2_pos: Vector3<f32> = Vector3::new(
                (next_angle).cos() * (cur_angle_vert).sin(),
                (cur_angle_vert).cos(),
                (next_angle).sin() * (cur_angle_vert).sin(),
            );

            // The upside down triangle from previous iteration
            let v_neg_pos: Vector3<f32> = Vector3::new(
                (prev_angle + (res_radians / 2.0)).cos() * (next_angle_vert).sin(),
                (next_angle_vert).cos(),
                (prev_angle + (res_radians / 2.0)).sin() * (next_angle_vert).sin(),
            );

            if v_int != 0 {
                let tri_pos_normal = (v1_pos - v0_pos).cross(v2_pos - v0_pos).normalize();
                vertices.push(Vertex {
                    position: v0_pos.into(),
                    normal: tri_pos_normal.into(),
                });
                vertices.push(Vertex {
                    position: v1_pos.into(),
                    normal: tri_pos_normal.into(),
                });
                vertices.push(Vertex {
                    position: v2_pos.into(),
                    normal: tri_pos_normal.into(),
                });
            }

            // SPDLOG_INFO("x: {}, y: {}, z: {}", v0_pos.x, v0_pos.y, v0_pos.z);
            // SPDLOG_INFO("x: {}, y: {}, z: {}", v1_pos.x, v1_pos.y, v1_pos.z);
            // SPDLOG_INFO("x: {}, y: {}, z: {}", v2_pos.x, v2_pos.y, v2_pos.z);

            if v_int != sized_res - 1 {
                let tri_neg_normal = (v_neg_pos - v0_pos).cross(v1_pos - v0_pos).normalize();
                vertices.push(Vertex {
                    position: v0_pos.into(),
                    normal: tri_neg_normal.into(),
                });
                vertices.push(Vertex {
                    position: v_neg_pos.into(),
                    normal: tri_neg_normal.into(),
                });
                vertices.push(Vertex {
                    position: v1_pos.into(),
                    normal: tri_neg_normal.into(),
                });
            }
        }
    }
    //debug!(
    //    "vertices: {:?}, vertex_capacity: {:?}",
    //    vertices.len(),
    //    vertex_capacity,
    //    //indices.len(),
    //    //index_capacity
    //);

    Ok(vertices)
}

pub fn generate_index_sphere(resolution: u8) -> anyhow::Result<(Vec<Vertex>, Vec<Index>), String> {
    let sized_res = resolution as u32;

    if sized_res <= 2 {
        return Err("sized_res is less than or equal to 2!".to_string());
    }

    let vertex_capacity = (sized_res * (sized_res - 1)) + 2;

    let mut vertices: Vec<Vertex> = Vec::with_capacity(vertex_capacity as usize);

    let index_capacity = (sized_res * (sized_res - 2) * 6) + sized_res * 6;

    let mut indices: Vec<Index> = Vec::with_capacity(index_capacity as usize);

    // angle of a triangle section of the circle in radians (divide by 2 for
    // half of a circle for vertical)
    let res_radians = 1.0 / (sized_res as f32 / (PI * 2.0));

    vertices.push(Vertex {
        position: [0.0, 1.0, 0.0],
        normal: [0.0, 1.0, 0.0],
    });

    // Iterate through vertices
    for v_int in 1..sized_res {
        // Iterate through the vertical levels of the icosphere
        for h_int in 0..sized_res {
            // Iterate through each vertex on the current Y plane of the sphere

            let v = v_int as f32;
            let h = h_int as f32;

            let h_offset = v * (res_radians / 2.0);

            // float prev_angle = ((h - 1) * res_radians) + h_offset;
            let cur_angle = (h * res_radians) + h_offset;
            // float next_angle = ((h + 1) * res_radians) + h_offset;

            // sized_res in radians is half because vertical is only half of a
            // circle
            let cur_angle_vert = v * (res_radians / 2.0);

            let v0_pos: Vector3<f32> = Vector3::new(
                (cur_angle).cos() * (cur_angle_vert).sin(),
                (cur_angle_vert).cos(),
                (cur_angle).sin() * (cur_angle_vert).sin(),
            );

            vertices.push(Vertex {
                position: v0_pos.into(),
                normal: [0.0, 0.0, 0.0],
            });
        }
    }
    vertices.push(Vertex {
        position: [0.0, -1.0, 0.0],
        normal: [0.0, -1.0, 0.0],
    });

    // has sized_res*2 elements
    // first triangle is the right side up second is upside down
    // Top and bottom have redundant 0,0,0 normals for the non-applicable
    // triangles

    let tri_normals_capacity = pow(sized_res, 2) * 2;
    let mut tri_normals: Vec<Vector3<f32>> =
        vec![(0.0, 0.0, 0.0).into(); tri_normals_capacity as usize];

    // first horizontal index of the 2nd to last layer of the triangle (actual
    // last layer is just 1 vert)
    let last_layer_idx = (pow(sized_res, 2) - (sized_res * 2)) + 1;

    // Generate face normals and indices
    for h_int in 0..sized_res {
        // Generate first layer and last layer
        {
            // Positive vertical and positive horizontal index into vertices array
            let v1_h0_idx = h_int + 1;
            let v1_h0: Vector3<f32> = vertices[v1_h0_idx as usize].position.into();

            let v1_h1_idx = (v1_h0_idx % sized_res) + 1;
            let v1_h1: Vector3<f32> = vertices[v1_h1_idx as usize].position.into();

            let v0: Vector3<f32> = vertices[0].position.into();

            indices.push(v1_h1_idx);
            indices.push(v1_h0_idx);
            indices.push(0);

            tri_normals[(h_int * 2) as usize] = (v1_h0 - v1_h1).cross(v0 - v1_h1).normalize();

            // v2= last layer
            let v2_h0_idx = h_int + last_layer_idx;
            let v2_h0: Vector3<f32> = vertices[v2_h0_idx as usize].position.into();
            let v2_h1_idx = ((h_int + 1) % sized_res) + last_layer_idx;
            let v2_h1: Vector3<f32> = vertices[v2_h1_idx as usize].position.into();

            // v3 = actual last vertex
            // This will be broken if vertex_capacity is broken
            let v3_idx = vertex_capacity - 1;
            let v3: Vector3<f32> = vertices[v3_idx as usize].position.into();

            indices.push(v3_idx);
            indices.push(v2_h0_idx);
            indices.push(v2_h1_idx);

            tri_normals[(((tri_normals_capacity - (sized_res * 2)) + (h_int * 2)) + 1) as usize] =
                (v2_h0 - v3).cross(v2_h1 - v3).normalize();
        }
    }
    // generate the middle layers
    for v_int in 0..sized_res - 2 {
        for h_int in 0..sized_res {
            let v0_h0_idx = h_int + (v_int * sized_res) + 1;
            let v0_h0: Vector3<f32> = vertices[v0_h0_idx as usize].position.into();

            let v0_h1_idx = ((h_int + 1) % sized_res) + (v_int * sized_res) + 1;
            let v0_h1: Vector3<f32> = vertices[v0_h1_idx as usize].position.into();

            let v1_h0_idx = h_int + ((v_int + 1) * sized_res) + 1;
            let v1_h0: Vector3<f32> = vertices[v1_h0_idx as usize].position.into();

            let v1_hneg1_idx = (((h_int as i64 - 1) + sized_res as i64) as u32 % sized_res)
                + ((v_int + 1) * sized_res)
                + 1;
            let v1_hneg1: Vector3<f32> = vertices[v1_hneg1_idx as usize].position.into();

            {
                // right side up triangle
                indices.push(v1_h0_idx);
                indices.push(v0_h0_idx);
                indices.push(v0_h1_idx);

                tri_normals[(((h_int + (v_int * sized_res)) * 2) + (sized_res * 2)) as usize] =
                    (v0_h0 - v1_h0).cross(v0_h1 - v1_h0).normalize();
            }

            {
                // upside down triangle
                indices.push(v1_h0_idx);
                indices.push(v1_hneg1_idx);
                indices.push(v0_h0_idx);

                tri_normals[(((h_int + (v_int * sized_res)) * 2) + (sized_res * 2) + 1) as usize] =
                    (v1_hneg1 - v1_h0).cross(v0_h0 - v1_h0).normalize();
            }
        }
    }

    // Iterate through vertices make averaged normals
    for v_int in 0..sized_res - 1 {
        // Iterate through the vertical levels of the icosphere
        for h_int in 0..sized_res {
            // Iterate through each vertex on the current Y plane of the sphere

            // Vertices are on the NEXT layer from the triangles
            let cur_vtx_idx = h_int + (v_int * sized_res) + 1;

            let v0_tri1_idx = (h_int + (v_int * sized_res)) * 2;
            let v0_tri2_idx = v0_tri1_idx + 1;
            let v1_tri3_idx = (((h_int + 1) % sized_res) + (v_int * sized_res)) * 2;

            let v2_tri4_idx = (((((h_int as i64 - 1) + sized_res as i64) as u32 % sized_res)
                + ((v_int + 1) * sized_res))
                * 2)
                + 1;

            let v3_tri5_idx = (h_int + ((v_int + 1) * sized_res)) * 2;
            let v3_tri6_idx = ((h_int + ((v_int + 1) * sized_res)) * 2) + 1;

            let mut sum = Vector3::new(0.0, 0.0, 0.0);

            sum += tri_normals[v0_tri1_idx as usize];
            sum += tri_normals[v0_tri2_idx as usize];
            sum += tri_normals[v1_tri3_idx as usize];
            sum += tri_normals[v2_tri4_idx as usize];
            sum += tri_normals[v3_tri5_idx as usize];
            sum += tri_normals[v3_tri6_idx as usize];
            vertices[cur_vtx_idx as usize].normal = sum.normalize().into();
        }
    }
    Ok((vertices, indices))
}
