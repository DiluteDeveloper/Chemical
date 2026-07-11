pub mod index_mesh;
pub mod point_light;
pub mod transform;
pub mod vertex_mesh;

pub type EntityID = u32;

pub use index_mesh::IndexMesh;
pub use point_light::PointLight;
pub use transform::Transform;
pub use vertex_mesh::VertexMesh;
