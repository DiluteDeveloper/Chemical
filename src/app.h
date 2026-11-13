#pragma once

#include <webgpu/webgpu_cpp.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Chemical {

    class App {

    public:
        static void Start();

        struct SetupWGPUResult {
            wgpu::Instance instance;
            wgpu::Adapter adapter;
            wgpu::Device device;
            wgpu::Queue queue;
        } wgpu;

        GLFWwindow* window;

        struct BufferInit { 
            uint32_t vertex_count; 
            wgpu::Buffer vertex_buffer; 
            wgpu::Buffer projection_buffer; 
            wgpu::Buffer view_buffer; 
            wgpu::BindGroupLayout view_projection_bind_group_layout;
            wgpu::BindGroup view_projection_bind_group;
        } buf;

        struct SurfaceConfig {
            wgpu::Surface surface;
            wgpu::TextureFormat format;
        } surface;

        wgpu::RenderPipeline pipeline;

        [[nodiscard]] static SetupWGPUResult SetupWGPU();
        [[nodiscard]] static GLFWwindow* SetupGLFW();

        struct SetupRenderPipelineParam {
            wgpu::Device device; 
            wgpu::TextureFormat surface_format; 
            wgpu::BindGroupLayout view_projection_bind_group_layout;
        };
        [[nodiscard]] static wgpu::RenderPipeline SetupRenderPipeline(const SetupRenderPipelineParam& p);

        struct ConfigureSurfaceParam {
            wgpu::Instance instance; 
            wgpu::Adapter adapter; 
            wgpu::Device device; 
            GLFWwindow* window;
        };
        [[nodiscard]] static SurfaceConfig ConfigureSurface(const ConfigureSurfaceParam& p);

        struct RenderPassParams {
            const wgpu::CommandEncoder encoder; 
            const wgpu::Queue queue; 
            const wgpu::RenderPipeline pipeline; 
            const wgpu::TextureView surface_view;
            const wgpu::TextureView depth_view;

            const wgpu::Buffer vertex_buffer;
            const uint32_t vertex_count;

            wgpu::BindGroup view_projection_bind_group;
        };
        static void RenderPass(const RenderPassParams& p);

        [[nodiscard]] static BufferInit InitialiseBuffers(wgpu::Queue queue, wgpu::Device device);

        struct Depth {
            wgpu::Texture texture;
            wgpu::TextureView view;
        } depth;
        [[nodiscard]] static Depth SetupDepthTexture(wgpu::Device device);

        static void ApplicationLoop(void* arg);

        [[nodiscard]] static glm::fmat4 GenerateProjectionMatrix(float width, float height, 
                float fov = 90, float near = 0.1f, float far = 1000.0f);
    };
}
