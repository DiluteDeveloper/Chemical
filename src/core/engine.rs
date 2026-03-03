use crate::core::app::{self, App};
use crate::core::state::State;
use std::sync::Arc;
use winit::{event_loop::EventLoopProxy, window::Window};

pub struct ChemicalEngine {
    state: State,
    event_loop_proxy: EventLoopProxy<app::ChemicalEvent>,
}

impl ChemicalEngine {
    pub(super) fn new(
        window: Arc<Window>,
        event_loop_proxy: Option<EventLoopProxy<app::ChemicalEvent>>,
    ) -> Option<Self> {
        Some(ChemicalEngine {
            state: pollster::block_on(State::new(window)).expect("Failed to initialise State"),
            event_loop_proxy: event_loop_proxy.expect("Event loop proxy was invalid"),
        })
    }

    pub(super) fn update(&mut self) {}
}
