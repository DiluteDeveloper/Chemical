use log::LevelFilter;

mod camera;

mod core;

mod geometry {
    pub mod sphere;
    pub mod vertex;
}
mod rendering {
    pub mod mesh;
    pub mod vertex;
}
mod utility {
    pub mod fps_counter;
}

use crate::core::app;

fn main() {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info) // or Debug, Trace
        .init();

    app::run().expect("App failed to run");
}
