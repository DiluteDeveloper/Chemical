// use crate::geometry::vertex;
// pub fn generate_cube(width: f32) -> ([vertex::Vertex; 8], [vertex::Index; 36]) {
//     let vertices: [vertex::Vertex; 8] = [
//         vertex::Vertex {
//             position: (-width, -width, -width).into(),
//             normal: (-1.0, -1.0, -1.0).into(),
//         },
//         vertex::Vertex {
//             position: (width, -width, -width).into(),
//             normal: (1.0, -1.0, -1.0).into(),
//         },
//         vertex::Vertex {
//             position: (-width, -width, width).into(),
//             normal: (-1.0, -1.0, 1.0).into(),
//         },
//         vertex::Vertex {
//             position: (width, -width, width).into(),
//             normal: (1.0, -1.0, 1.0).into(),
//         },
//         vertex::Vertex {
//             position: (-width, width, -width).into(),
//             normal: (-1.0, 1.0, -1.0).into(),
//         },
//         vertex::Vertex {
//             position: (width, width, -width).into(),
//             normal: (1.0, 1.0, -1.0).into(),
//         },
//         vertex::Vertex {
//             position: (-width, width, width).into(),
//             normal: (-1.0, 1.0, 1.0).into(),
//         },
//         vertex::Vertex {
//             position: (width, width, width).into(),
//             normal: (1.0, 1.0, 1.0).into(),
//         },
//     ];
//     let indices: [vertex::Index; 36] = [
//         2, 0, 1, 3, 2, 1, // Bottom face
//         4, 6, 7, 5, 4, 7, // Top face
//         0, 4, 5, 1, 0, 5, // Front face
//         2, 3, 7, 2, 7, 6, // Back face
//         2, 6, 4, 2, 4, 0, // Left face
//         1, 5, 7, 1, 7, 3, // Right face
//     ];
//
//     (vertices, indices)
// }
