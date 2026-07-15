use log::LevelFilter;

mod core;
mod input;

use crate::core::app;

fn main() {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info)
        .filter_module("wgpu_hal", LevelFilter::Off)
        .filter_module("wgpu_core", LevelFilter::Off)
        .filter_module("naga", LevelFilter::Off)
        .init();

    app::run().expect("App failed to run");
}
