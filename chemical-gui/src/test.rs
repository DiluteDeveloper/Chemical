#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
pub struct Counter {
    value: i64,
}
impl Counter {
    pub fn new() -> Self {
        Self { value: 0 }
    }
}
use iced_widget::{Column, button, column, text};
use iced_winit::core::{Rectangle, Theme};

// update view logic
// display interface
// process the user interactions and obtain our messages
// Update our state by processing each message

impl Counter {
    // run before display
    pub fn view(&self) -> Column<'_, Message> {
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
