#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
pub struct Counter {
    value: i64,
    render_frame: SceneProgram,
}
impl Counter {
    pub fn new(camera: Arc<Mutex<Camera>>, renderer: Arc<Mutex<chemical_api::Renderer>>) -> Self {
        Self {
            value: 0,
            render_frame: SceneProgram {
                renderer: renderer,
                camera: camera,
            },
        }
    }
}

use std::sync::{Arc, Mutex};

use chemical_api::Camera;
use iced_wgpu::Renderer;
use iced_widget::{Column, button, column, shader, text};
use iced_winit::core::{Length, Theme};

use crate::frame::{RenderFrame, SceneProgram};

// update view logic
// display interface
// process the user interactions and obtain our messages
// Update our state by processing each message

impl Counter {
    // run before display
    pub fn view(&self) -> Column<'_, Message, Theme, Renderer> {
        column![
            shader(&self.render_frame)
                .width(Length::FillPortion(3))
                .height(Length::Fill),
            button("+").on_press(Message::Increment),
            text(self.value),
            button("-").on_press(Message::Decrement)
        ]
    }
    pub fn update(&mut self, message: Message) {
        match message {
            Message::Increment => {
                self.value += 1;
            }
            Message::Decrement => {
                self.value -= 1;
            }
        }
    }
}
