use crate::scene::types::vertex_mesh::Vertex;

pub fn generate_vertex_cube(extents: cgmath::Vector3<f32>) -> [Vertex; 36] {
    [
        // Front face (+Z)
        Vertex {
            position: (-extents.x, -extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, extents.z).into(),
            normal: (0.0, 0.0, 1.0).into(),
        },
        // Back face (-Z)
        Vertex {
            position: (extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 0.0, -1.0).into(),
        },
        // Right face (+X)
        Vertex {
            position: (extents.x, -extents.y, extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, -extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, -extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, -extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, extents.z).into(),
            normal: (1.0, 0.0, 0.0).into(),
        },
        // Left face (-X)
        Vertex {
            position: (-extents.x, -extents.y, -extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, -extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, -extents.z).into(),
            normal: (-1.0, 0.0, 0.0).into(),
        },
        // Top face (+Y)
        Vertex {
            position: (-extents.x, extents.y, extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, extents.y, -extents.z).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        // Bottom face (-Y)
        Vertex {
            position: (-extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, -extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, -extents.y, extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, -extents.y, extents.z).into(),
            normal: (0.0, -1.0, 0.0).into(),
        },
    ]
}
