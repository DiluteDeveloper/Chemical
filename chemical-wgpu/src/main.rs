use log::LevelFilter;

mod camera;
mod core;
mod renderer;

use crate::core::app;
pub use camera::{Camera, CameraController};
pub use renderer::Renderer;

fn main() {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info) // or Debug, Trace
        .init();

    app::run().expect("App failed to run");
}
