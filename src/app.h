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

        GLFWwindow* glfw_window;
        
        void SetupWGPU();
        void SetupGLFW();
        static void ApplicationLoop(void* arg);
    };
}
