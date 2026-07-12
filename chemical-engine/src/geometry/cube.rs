use glam::Vec3;

use crate::scene::types::mesh::Vertex;

pub fn generate_vertex_cube(extents: Vec3) -> [Vertex; 36] {
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
