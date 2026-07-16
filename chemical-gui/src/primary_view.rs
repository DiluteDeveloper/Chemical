#[derive(Debug, Clone, Copy)]
pub enum Message {
    Increment,
    Decrement,
}
use iced_wgpu::Renderer;
use iced_widget::{column, container, row, text};
use iced_winit::core::{Element, Length::Fill, Theme};

use super::ViewportRegion;

const LEFT_PANEL_WIDTH: u32 = 240;
const RIGHT_PANEL_WIDTH: u32 = 240;
const TOP_PANEL_HEIGHT: u32 = 120;
const BOTTOM_PANEL_HEIGHT: u32 = 120;

/// The GUI panel covering the entire window.
#[derive(Debug)]
pub struct PrimaryView {}

impl PrimaryView {
    pub fn new() -> Self {
        Self {}
    }

    /// Returns the size and offset for the viewport relative to `window_size`.  
    /// Err if `window_size` is too small to fit a viewport region.
    pub fn get_viewport_region(
        window_size: &iced_winit::winit::dpi::PhysicalSize<u32>,
    ) -> anyhow::Result<ViewportRegion> {
        debug_assert!(window_size.width > 0 && window_size.height > 0);
        if window_size.width <= LEFT_PANEL_WIDTH + RIGHT_PANEL_WIDTH
            || window_size.height <= TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT
        {
            anyhow::bail!("Window size is too small to fit the viewport region in GUI layout");
        }
        Ok(ViewportRegion {
            width: window_size.width - LEFT_PANEL_WIDTH - RIGHT_PANEL_WIDTH,
            height: window_size.height - TOP_PANEL_HEIGHT - BOTTOM_PANEL_HEIGHT,
            offset_x: LEFT_PANEL_WIDTH,
            offset_y: TOP_PANEL_HEIGHT,
        })
    }
    /// Returns the primary GUI view with a gap for the viewport.
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

        let middle_row = row![left_panel, viewport, right_panel].height(Fill);

        column![top_panel, middle_row, bottom_panel]
            .width(Fill)
            .height(Fill)
            .into()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    #[should_panic]
    fn test_get_viewport_region_with_window_size_0_panics() {
        let _ = PrimaryView::get_viewport_region(&iced_winit::winit::dpi::PhysicalSize::<u32> {
            width: 0,
            height: 0,
        });
    }
    #[test]
    fn test_get_viewport_region_with_window_size_less_than_minimum_err() {
        assert!(
            PrimaryView::get_viewport_region(&iced_winit::winit::dpi::PhysicalSize::<u32> {
                width: LEFT_PANEL_WIDTH + RIGHT_PANEL_WIDTH,
                height: TOP_PANEL_HEIGHT + BOTTOM_PANEL_HEIGHT,
            })
            .is_err(),
        );
    }
}
