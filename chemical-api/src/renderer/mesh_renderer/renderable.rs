pub trait Renderable {
    fn bind(&self, render_pass: &mut wgpu::RenderPass) {}
    fn draw(&self, render_pass: &mut wgpu::RenderPass) {}
}
