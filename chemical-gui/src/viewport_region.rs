/// Parameters for where the viewport should be rendered on the window.
#[derive(Debug)]
pub struct ViewportRegion {
    pub width: u32,
    pub height: u32,
    pub offset_x: u32,
    pub offset_y: u32,
}
