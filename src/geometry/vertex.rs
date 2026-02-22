// Makes the memory layout of the struct C-like, rust normally optimizes struct memory layout
#[repr(C)]
// Automatically has traits Copy Clone and Debug
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct Vertex {
    pub position: [f32; 3],
    pub normal: [f32; 3],
}
pub type Index = u16;
