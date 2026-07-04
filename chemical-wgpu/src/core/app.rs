use std::sync::Arc;

use winit::{
    application::ApplicationHandler,
    event::*,
    event_loop::{ActiveEventLoop, EventLoop, EventLoopProxy},
    window::Window,
};

use crate::core::engine;

struct App {
    engine: Option<engine::ChemicalEngine>,
    // Gets taken when resumed() is called
    event_loop_proxy: Option<EventLoopProxy<engine::ChemicalEvent>>,
}

impl App {
    fn new(event_loop_proxy: EventLoopProxy<engine::ChemicalEvent>) -> Self {
        App {
            engine: None,
            event_loop_proxy: Some(event_loop_proxy),
        }
    }
}

impl ApplicationHandler<engine::ChemicalEvent> for App {
    fn resumed(&mut self, event_loop: &ActiveEventLoop) {
        let all_monitors = event_loop.available_monitors().collect::<Vec<_>>();
        let target_monitor = all_monitors.get(0).unwrap();
        let window_attributes = Window::default_attributes()
            .with_title(String::from(format!(
                "ChemicalRS {}",
                env!("CARGO_PKG_VERSION")
            )))
            .with_position(target_monitor.position())
            .with_inner_size(winit::dpi::LogicalSize::new(1280.0, 720.0));

        self.engine = Some(
            engine::ChemicalEngine::new(
                Arc::new(
                    event_loop
                        .create_window(window_attributes)
                        .expect("Failed to create window"),
                ),
                self.event_loop_proxy.take(),
            )
            .expect("Failed to initialise ChemicalEngine"),
        );
    }

    fn user_event(&mut self, event_loop: &ActiveEventLoop, event: engine::ChemicalEvent) {
        self.engine
            .as_mut()
            .expect("Chemical event triggered before engine initialisation")
            .chemical_event(&event, &event_loop);
    }

    fn window_event(
        &mut self,
        event_loop: &ActiveEventLoop,
        _window_id: winit::window::WindowId,
        event: WindowEvent,
    ) {
        self.engine
            .as_mut()
            .expect("Window event triggered before engine initialisation")
            .window_event(&event, &event_loop);
    }
    fn device_event(
        &mut self,
        event_loop: &ActiveEventLoop,
        _device_id: winit::event::DeviceId,
        event: DeviceEvent,
    ) {
        self.engine
            .as_mut()
            .expect("Device event triggered before engine initialisation")
            .device_event(&event, &event_loop);
    }
}

pub fn run() -> anyhow::Result<()> {
    let event_loop = EventLoop::<engine::ChemicalEvent>::with_user_event().build()?;
    let mut app = App::new(event_loop.create_proxy());
    event_loop.run_app(&mut app)?;

    Ok(())
}
