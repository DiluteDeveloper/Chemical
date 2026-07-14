use log::LevelFilter;

mod core;
mod input;

use crate::core::app;

fn main() {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info) // or Debug, Trace
        .init();

    app::run().expect("App failed to run");
}
