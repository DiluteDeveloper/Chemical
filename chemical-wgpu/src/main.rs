use log::LevelFilter;

mod camera;

mod core;
mod geometry;
mod rendering;
mod utility;

use crate::core::app;

fn main() {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info) // or Debug, Trace
        .init();

    app::run().expect("App failed to run");
}
