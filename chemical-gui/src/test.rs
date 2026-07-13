#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
#[derive(Default)]
pub struct Counter {
    value: i64,
}

use iced_wgpu::Renderer;
use iced_widget::{Column, button, column, text};
use iced_winit::core::Theme;

// update view logic
// display interface
// process the user interactions and obtain our messages
// Update our state by processing each message

impl Counter {
    // run before display
    pub fn view(&self) -> Column<'_, Message, Theme, Renderer> {
        column![
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
