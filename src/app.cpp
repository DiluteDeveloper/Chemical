#include <spdlog/spdlog.h>
#include <webgpu/webgpu_glfw.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "app.h"

#include "utils/webgpu_utils.h"

namespace Chemical {

    App::App() {
        try {
            SetupWGPU();
            SetupGLFW();

            Utils::WebGPU::PrintAdapterInfo(webgpu_adapter);
#ifdef __EMSCRIPTEN__
        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector src;
        src.selector = "#canvas";
        wgpu::SurfaceDescriptor desc;
        desc.nextInChain = &src;
        wgpu::Surface surface = wgpu.instance.CreateSurface(&desc);
#else
        webgpu_surface = wgpu::glfw::CreateSurfaceForWindow(webgpu_instance, glfw_window);
#endif
        wgpu::SurfaceCapabilities capabilities;
        webgpu_surface.GetCapabilities(webgpu_adapter, &capabilities);
        wgpu::SurfaceConfiguration config = {};
        config.device = webgpu_device;
        config.format = capabilities.formats[0];
        config.width = 1280;
        config.height = 720;
        if(capabilities.presentModeCount == 0) throw std::exception();
        config.presentMode = capabilities.presentModes[0];
        config.alphaMode = wgpu::CompositeAlphaMode::Auto;
        webgpu_surface.Configure(&config);

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(main_loop_callback, &core, 0, true);
#else
        while(!glfwWindowShouldClose(glfw_window)) {
            ApplicationLoop(this);
            webgpu_surface.Present();
        }
#endif

        } catch (const std::exception& e) {
            spdlog::critical("Error occurred during application startup: {}", e.what());
        }
    }

    App::~App() {
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
    }

    void App::SetupWGPU() {

        webgpu_instance = wgpu::CreateInstance(nullptr);
        
        // Adapter ----------------------------------------
        
        spdlog::trace("Requesting WebGPU adapter.");

        webgpu_instance.RequestAdapter(nullptr, wgpu::CallbackMode::AllowProcessEvents, [this](wgpu::RequestAdapterStatus status, wgpu::Adapter new_adapter, wgpu::StringView message) {
            if (status == wgpu::RequestAdapterStatus::Success)
                webgpu_adapter = new_adapter;
            else
                spdlog::error("WebGPU adapter request failed: {}", message.data);
        });

#ifdef __EMSCRIPTEN__
        while(!adapter) {
            emscripten_sleep(10); 
            instance.ProcessEvents();
        }
#else
        webgpu_instance.ProcessEvents();
#endif


        // Device ----------------------------------------
        
        spdlog::trace("Requesting WebGPU device.");

        wgpu::DeviceDescriptor device_descriptor;
        device_descriptor.SetDeviceLostCallback(wgpu::CallbackMode::AllowProcessEvents, [](const wgpu::Device&, wgpu::DeviceLostReason, wgpu::StringView message) {
            spdlog::error("WebGPU device lost: {}", message.data);
        });
        device_descriptor.SetUncapturedErrorCallback([](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView message) {
            spdlog::error("WebGPU device uncaptured error: {} with error type: ", message.data, ((int)type));
        });

        webgpu_adapter.RequestDevice(&device_descriptor, wgpu::CallbackMode::AllowProcessEvents, [this](wgpu::RequestDeviceStatus status, wgpu::Device new_device, wgpu::StringView message) {
          if (status == wgpu::RequestDeviceStatus::Success)
              webgpu_device = new_device;
          else
              spdlog::error("WebGPU device request failed: {}", message.data);
        });

#ifdef __EMSCRIPTEN__
        while(!device) {
            emscripten_sleep(10); 
            instance.ProcessEvents();
        }
#else
        webgpu_instance.ProcessEvents();
#endif


        webgpu_queue = webgpu_device.GetQueue();

    }
    void App::SetupGLFW() {

        glfwSetErrorCallback([](int code, const char* message) {
                spdlog::error("GLFW error: {} - {}", code, message);
        });

        // Wayland is used by default but only X11 works on WSL
#ifdef FORCE_X11
        glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

        glfwInit();

        if(!glfwInit())
            throw std::runtime_error("Failed to initialise GLFW!");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        glfw_window = glfwCreateWindow(1280, 720, "ChemicalMC_PK", nullptr, nullptr);

        if(!glfw_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }
    }
    void App::ApplicationLoop(void* arg) {
        App& app = *reinterpret_cast<App*>(arg);

        wgpu::SurfaceTexture surfaceTexture;
        app.webgpu_surface.GetCurrentTexture(&surfaceTexture);
        // wgpu::CommandBuffer commandBuffer =
        //     wgpu::CreateCommandBuffer(surfaceTexture.texture.CreateView(), pingpong);
        // queue.Submit(1, &commandBuffer);
        // pingpong = (pingpong + 1) % 2;

        glfwPollEvents();
    }
}
