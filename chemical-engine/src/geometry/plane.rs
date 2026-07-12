use crate::scene::types::mesh::Vertex;
pub fn generate_vertex_plane(extents: cgmath::Vector2<f32>) -> [Vertex; 6] {
    [
        Vertex {
            position: (extents.x, 0.0, -extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, 0.0, -extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, 0.0, extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, 0.0, extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (extents.x, 0.0, -extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
        Vertex {
            position: (-extents.x, 0.0, extents.y).into(),
            normal: (0.0, 1.0, 0.0).into(),
        },
    ]
}
