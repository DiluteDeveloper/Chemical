pub mod directional_light;
pub mod mesh;
pub mod point_light;
pub mod transform;

pub type EntityID = u32;

pub use directional_light::DirectionalLight;
pub use mesh::StaticMesh;
pub use point_light::PointLight;
pub use transform::Transform;
