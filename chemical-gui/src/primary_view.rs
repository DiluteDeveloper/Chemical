#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
use iced_wgpu::{Renderer, core::window};
use iced_widget::{column, container, row, text};
use iced_winit::core::{
    Element,
    Length::{self, Fill},
    Theme,
};

use crate::ViewportRegion;

const LEFT_PANEL_WIDTH: u32 = 240;
const RIGHT_PANEL_WIDTH: u32 = 240;
const TOP_PANEL_HEIGHT: u32 = 120;
const BOTTOM_PANEL_HEIGHT: u32 = 120;

pub struct PrimaryView {}

impl PrimaryView {
    pub fn new(window_size: &iced_winit::winit::dpi::PhysicalSize<u32>) -> (Self, ViewportRegion) {
        (
            Self {},
            ViewportRegion {
                width: window_size.width - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH,
                height: window_size.height - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT,
                offset_x: LEFT_PANEL_WIDTH,
                offset_y: TOP_PANEL_HEIGHT,
            },
        )
    }
    pub fn resize(window_size: &iced_winit::winit::dpi::PhysicalSize<u32>) -> ViewportRegion {
        ViewportRegion {
            width: window_size.width - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH,
            height: window_size.height - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT,
            offset_x: LEFT_PANEL_WIDTH,
            offset_y: TOP_PANEL_HEIGHT,
        }
    }
    pub fn view(&self) -> Element<'_, Message, Theme, Renderer> {
        let top_panel = container(text("File   Edit   View   Tools"))
            .padding(8)
            .width(Fill)
            .height(TOP_PANEL_HEIGHT)
            .style(container::bordered_box);

        let left_panel = container(text("Hierarchy"))
            .padding(10)
            .width(LEFT_PANEL_WIDTH)
            .height(Fill)
            .style(container::bordered_box);

        let right_panel = container(text("Inspector"))
            .padding(10)
            .width(RIGHT_PANEL_WIDTH)
            .height(Fill)
            .style(container::bordered_box);

        let bottom_panel = container(text("Console"))
            .padding(8)
            .width(Fill)
            .height(BOTTOM_PANEL_HEIGHT)
            .style(container::bordered_box);

        let viewport = container("")
            .width(Fill)
            .height(Fill)
            .center_x(Fill)
            .center_y(Fill);

        // // Center the fixed 720x480 viewport in whatever space is left
        // // between the two side panels, regardless of window size.
        // let viewport_area = container(viewport).center_x(Fill).center_y(Fill);

        let middle_row = row![left_panel, viewport, right_panel].height(Fill);

        column![top_panel, middle_row, bottom_panel]
            .width(Fill)
            .height(Fill)
            .into()
    }
}
