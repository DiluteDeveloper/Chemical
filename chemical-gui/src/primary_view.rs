#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
use iced_wgpu::Renderer;
use iced_widget::{column, container, row, text};
use iced_winit::core::{
    Element,
    Length::{self, Fill},
    Theme,
};

use crate::ViewportRegion;

pub struct PrimaryView {}

impl PrimaryView {
    pub fn new() -> (Self, ViewportRegion) {
        (
            Self {},
            ViewportRegion {
                width: 500,
                height: 500,
                offset_x: 0,
                offset_y: 0,
            },
        )
    }
    pub fn resize(size: &iced_winit::winit::dpi::PhysicalSize<u32>) -> ViewportRegion {
        ViewportRegion {
            width: 500,
            height: 500,
            offset_x: 500,
            offset_y: 500,
        }
    }
    pub fn view(&self) -> Element<'_, Message, Theme, Renderer> {
        // Top menu / toolbar
        let top_bar = container(text("File   Edit   View   Tools"))
            .padding(8)
            .width(Fill)
            .height(120)
            .style(container::bordered_box);

        // Left panel: scene hierarchy, asset browser, etc.
        let left_panel = container(text("Hierarchy"))
            .padding(10)
            .width(280)
            .height(Fill)
            .style(container::bordered_box);

        // Right panel: inspector / properties
        let right_panel = container(text("Inspector"))
            .padding(10)
            .width(280)
            .height(Fill)
            .style(container::bordered_box);

        // Bottom bar: console / status / timeline
        let bottom_bar = container(text("Console"))
            .padding(8)
            .width(Fill)
            .height(120)
            .style(container::bordered_box);

        let viewport = container("")
            .width(Length::Fixed(1280.0))
            .height(Length::Fixed(720.0))
            .center_x(Length::Fixed(1280.0))
            .center_y(Length::Fixed(720.0));

        // Center the fixed 720x480 viewport in whatever space is left
        // between the two side panels, regardless of window size.
        let viewport_area = container(viewport).center_x(Fill).center_y(Fill);

        let middle_row = row![left_panel, viewport_area, right_panel].height(Fill);

        column![top_bar, middle_row, bottom_bar]
            .width(Fill)
            .height(Fill)
            .into()
    }
}
