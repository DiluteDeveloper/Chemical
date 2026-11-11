#pragma once

#include <webgpu/webgpu_cpp.h>
#include <GLFW/glfw3.h>

namespace Chemical {

    class App {

    public:
        App();
        ~App();

    private:

        wgpu::Instance webgpu_instance;
        wgpu::Adapter webgpu_adapter;
        wgpu::Device webgpu_device;
        wgpu::Queue webgpu_queue;
        wgpu::Surface webgpu_surface;
        wgpu::TextureFormat webgpu_surface_format;
        wgpu::RenderPipeline pipeline;

        uint32_t vertex_count = 0;
        wgpu::Buffer vertex_buffer;

        GLFWwindow* glfw_window;
        
        void SetupWGPU();
        void SetupGLFW();
        void SetupRenderPipeline();
        void ConfigureSurface();
        void RenderPass(const wgpu::TextureView& view);
        void ReadBackBuffer();
        void InitialiseBuffers();

        static void ApplicationLoop(void* arg);
    };
}
