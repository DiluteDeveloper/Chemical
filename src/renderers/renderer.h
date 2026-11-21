#pragma once

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <entt/entity/fwd.hpp>

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
        friend class StaticBlockRenderer;
        Renderer(GLFWwindow* window, entt::registry& registry);
        ~Renderer();
        void Render();

    private:

        wgpu::Buffer view_buffer;
        wgpu::Instance instance;
        wgpu::Adapter adapter;
        wgpu::Device device;
        wgpu::Queue queue;
        wgpu::TextureFormat surface_format;
        wgpu::Surface surface;
        wgpu::BindGroupLayout view_projection_layout;

        PrimaryRenderPass primary_pass;
        ImguiRenderPass imgui_pass;

        wgpu::BindGroup camera_group;
        entt::registry* registry;

        void InitialiseWebGPU(GLFWwindow* window);

        std::tuple<wgpu::Buffer, wgpu::BindGroup, wgpu::BindGroupLayout> InitialiseViewProjection();
        std::tuple<wgpu::Texture, wgpu::TextureView> CreateDepthTexture();

        std::tuple<wgpu::SurfaceTexture, wgpu::TextureView> GetNextSurfaceViewData();
        void InitialisePrimaryRenderPass(wgpu::TextureView depth_texture_view);
        void InitialiseImguiRenderPass();
        void ExecuteImguiRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view);
        void ExecutePrimaryRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view);
        void InitialiseImgui(GLFWwindow* window);
        void AwaitAsync(const wgpu::Future& future);
    };

}
