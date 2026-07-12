use crate::scene::types::{
    Mesh,
    mesh::{Index, Vertex},
};

pub fn gltf_load(path: &str, is_lit: bool) -> Option<Vec<Mesh>> {
    let (gltf, buffers, _) = gltf::import(path).ok()?;
    let mut meshes: Vec<Mesh> = Vec::new();
    for mesh in gltf.meshes() {
        let mut vertices: Vec<Vertex> = Vec::new();
        let mut indices: Vec<Index> = Vec::new();
        for primitive in mesh.primitives() {
            let reader = primitive.reader(|buffer| Some(&buffers[buffer.index()]));
            if let Some(pos) = reader.read_positions() {
                if let Some(norm) = reader.read_normals() {
                    for vertex_position in pos {
                        vertices.push(Vertex {
                            position: vertex_position.into(),
                            normal: (0.0, 0.0, 0.0).into(),
                        });
                    }
                    for (i, vertex_normal) in norm.enumerate() {
                        vertices.get_mut(i)?.normal = vertex_normal.into();
                    }
                }
            }
            if let Some(g_indices) = reader.read_indices() {
                for index in g_indices.into_u32() {
                    indices.push(index);
                }
            }
        }
        if indices.len() == 0 {
            meshes.push(Mesh::new(&vertices, None, is_lit))
        } else {
            meshes.push(Mesh::new(&vertices, Some(&indices), is_lit))
        }
    }
    Some(meshes)
}
