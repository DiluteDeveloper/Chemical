#pragma once

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>
#include <entt/entity/fwd.hpp>

namespace Chemical {

    struct RendererInitData {
        wgpu::Instance instance;
        wgpu::Adapter adapter;
        wgpu::Device device;
        wgpu::Queue queue;
        wgpu::TextureFormat surface_format;
        wgpu::Surface surface;
        wgpu::BindGroupLayout view_projection_layout;
    };
    struct PrimaryRenderPass {
        wgpu::RenderPassColorAttachment color;
        wgpu::RenderPassDepthStencilAttachment depth;
        wgpu::RenderPassDescriptor descriptor;
    };
    struct ImguiRenderPass {
        wgpu::RenderPassColorAttachment color;
        wgpu::RenderPassDescriptor descriptor;

    };
    namespace Renderer {
        extern void Initialise(GLFWwindow* window, entt::registry& registry);
        extern void Shutdown();
        extern void Render(entt::registry& registry);

        // This is a temporary solution until I update the renderer
        // to be responsible for updating the view matrix buffer
        extern RendererInitData init;
        extern wgpu::Buffer view_buffer;
    };

}
