#pragma once
#include "static_block_renderer.h"
#include "renderer_data.h"
#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
namespace Chemical {

    struct PrimaryRenderPass {
        wgpu::RenderPassColorAttachment color;
        wgpu::RenderPassDepthStencilAttachment depth;
        wgpu::RenderPassDescriptor descriptor;
    };
    struct ImguiRenderPass {
        wgpu::RenderPassColorAttachment color;
        wgpu::RenderPassDescriptor descriptor;

    };

    class Renderer {
    public:

        void Initialise(GLFWwindow* window);
        void Shutdown();
        void Render();

        RendererInitData init;
        wgpu::Buffer view_buffer;
    private:

        void ExecutePrimaryRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view);
        void ExecuteImguiRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view);

        void InitialisePrimaryRenderPass(wgpu::TextureView depth_texture_view);
        void InitialiseImguiRenderPass();

        PrimaryRenderPass primary_pass;
        ImguiRenderPass imgui_pass;

        wgpu::BindGroup camera_group;

        StaticBlockRenderer static_block_renderer;
    };
}
