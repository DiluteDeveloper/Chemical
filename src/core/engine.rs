use crate::core::app;
use crate::core::state;

struct ChemicalEngine {
    app: app::App,
    state: state::State,
}

use winit::event_loop::EventLoop;

impl ChemicalEngine {
    fn new(proxy: EventLoopProxy<MyEvent>) -> Self {
        let event_loop = EventLoop::<app::MyEvent>::with_user_event().build()?;
        let mut app = app::App::new(event_loop.create_proxy());
        event_loop.run_app(&mut app)?;

        ChemicalEngine {
            app: app::App::new(event_loop.create_proxy()),
            state: state::State::new(app.window),
        }
    }
}

fn run() -> anyhow::Result<()> {
    let event_loop = EventLoop::<app::MyEvent>::with_user_event().build()?;
    let mut app = app::App::new(event_loop.create_proxy());
    event_loop.run_app(&mut app)?;

    Ok(())
}
