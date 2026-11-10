#include <spdlog/spdlog.h>
#include <webgpu/webgpu_glfw.h>
#include <fstream>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "app.h"

#include "utils/webgpu_utils.h"

namespace Chemical {

    App::App() {
        spdlog::set_level(spdlog::level::trace);
        SetupWGPU();
        SetupGLFW();
        ConfigureSurface();
        SetupRenderPipeline();

        Utils::WebGPU::PrintAdapterInfo(webgpu_adapter);


#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(ApplicationLoop, this, 0, true);
#else
        while(!glfwWindowShouldClose(glfw_window)) {
            ApplicationLoop(this);
        }
#endif


    }

    App::~App() {
        glfwDestroyWindow(glfw_window);
        glfwTerminate();
    }

    void App::ApplicationLoop(void* arg) {
        App& app = *reinterpret_cast<App*>(arg);

        auto[surface_texture, texture_view] = Utils::WebGPU::GetNextSurfaceViewData(app.webgpu_surface);
        if(!texture_view) return;

        app.RenderPass(texture_view);

#ifndef __EMSCRIPTEN__
        app.webgpu_surface.Present();
#endif
        // wgpu::CommandBuffer commandBuffer =
        //     wgpu::CreateCommandBuffer(surfaceTexture.texture.CreateView(), pingpong);
        // queue.Submit(1, &commandBuffer);
        // pingpong = (pingpong + 1) % 2;

        glfwPollEvents();
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
        while(!webgpu_adapter) {
            emscripten_sleep(10); 
            webgpu_instance.ProcessEvents();
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
        while(!webgpu_device) {
            emscripten_sleep(10); 
            webgpu_instance.ProcessEvents();
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
    void App::ConfigureSurface() {

#ifdef __EMSCRIPTEN__
        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector src;
        src.selector = "#canvas";
        wgpu::SurfaceDescriptor desc;
        desc.nextInChain = &src;
        webgpu_surface = webgpu_instance.CreateSurface(&desc);
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
        webgpu_surface_format = config.format;
    }
    void App::RenderPass(const wgpu::TextureView& view) {

        wgpu::CommandEncoder encoder = webgpu_device.CreateCommandEncoder(nullptr);

        wgpu::RenderPassColorAttachment color;
        color.view = view;
        color.loadOp = wgpu::LoadOp::Clear;
        color.storeOp = wgpu::StoreOp::Store;
        color.clearValue = wgpu::Color{0.4,0.15,0.2,1.0};

        wgpu::RenderPassDescriptor rp_desc;
        rp_desc.colorAttachmentCount = 1;
        rp_desc.colorAttachments = &color;

        wgpu::RenderPassEncoder render_pass = encoder.BeginRenderPass(&rp_desc);
    
        render_pass.SetPipeline(pipeline);
        render_pass.Draw(3, 1, 0, 0);
        render_pass.End();

        wgpu::CommandBuffer cmd = encoder.Finish(nullptr);

        webgpu_queue.Submit(1, &cmd);
    }
    void App::SetupRenderPipeline() {

		std::ifstream shader_file("res/shaders/main.wgsl");
        if (!shader_file.is_open())
            throw std::runtime_error("Failed to read file: res/shaders/main.wgsl");

        std::string shader_source((std::istreambuf_iterator<char>(shader_file)),
            std::istreambuf_iterator<char>());

        wgpu::ShaderSourceWGSL shader_code({nullptr, shader_source.data()});

        wgpu::ShaderModuleDescriptor shader_desc;
        shader_desc.nextInChain = &shader_code;

        wgpu::ShaderModule shader_module = webgpu_device.CreateShaderModule(&shader_desc);

        wgpu::RenderPipelineDescriptor rp_desc;

        // rp_desc.vertex.bufferCount = 0;
        // rp_desc.vertex.buffers = nullptr;
        rp_desc.vertex.module = shader_module;
        rp_desc.vertex.entryPoint = "vs_main";
        rp_desc.vertex.constantCount = 0;
        rp_desc.vertex.constants = nullptr;

        rp_desc.primitive.topology = wgpu::PrimitiveTopology::TriangleList;
        rp_desc.primitive.stripIndexFormat = wgpu::IndexFormat::Undefined;
        rp_desc.primitive.frontFace = wgpu::FrontFace::CCW;
        rp_desc.primitive.cullMode = wgpu::CullMode::None;

        wgpu::FragmentState frag;
        frag.module = shader_module;
        frag.entryPoint = "fs_main";
        frag.constantCount = 0;
        frag.constants = nullptr;

        rp_desc.fragment = &frag;

        rp_desc.depthStencil = nullptr;

        wgpu::BlendState blend;
        blend.color.srcFactor = wgpu::BlendFactor::SrcAlpha;
        blend.color.dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha;
        blend.color.operation = wgpu::BlendOperation::Add;
        blend.alpha.srcFactor = wgpu::BlendFactor::Zero;
        blend.alpha.dstFactor = wgpu::BlendFactor::One;
        blend.alpha.operation = wgpu::BlendOperation::Add;

        wgpu::ColorTargetState color_target;
        color_target.format = webgpu_surface_format;
        color_target.blend = &blend;
        color_target.writeMask = wgpu::ColorWriteMask::All;

        frag.targetCount = 1;
        frag.targets= &color_target;

        rp_desc.multisample.count = 1;
        rp_desc.multisample.mask = ~0u;

        rp_desc.multisample.alphaToCoverageEnabled = false;

        rp_desc.layout = nullptr;

        pipeline = webgpu_device.CreateRenderPipeline(&rp_desc);
    }
}
