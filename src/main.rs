use log::LevelFilter;

use winit::event_loop::EventLoop;

pub mod app;
mod camera;
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
mod state;
//mod texture;
use app::App;

pub fn run() -> anyhow::Result<()> {
    env_logger::Builder::new()
        .filter_level(LevelFilter::Info) // or Debug, Trace
        .init();

    let event_loop = EventLoop::<app::MyEvent>::with_user_event().build()?;
    let mut app = App::new(event_loop.create_proxy());
    event_loop.run_app(&mut app)?;

    Ok(())
}

fn main() {
    run().unwrap();
}
