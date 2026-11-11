#include <cstdint>
#include <iterator>
#include <spdlog/spdlog.h>
#include <webgpu/webgpu_cpp.h>
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

        // ReadBackBuffer();

        InitialiseBuffers();

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
        app.webgpu_instance.ProcessEvents();
    }
    void App::SetupWGPU() {

        webgpu_instance = wgpu::CreateInstance(nullptr);
        
        // Adapter ----------------------------------------
        
        spdlog::trace("Requesting WebGPU adapter.");

        wgpu::Future adapter_future = webgpu_instance.RequestAdapter(nullptr, wgpu::CallbackMode::WaitAnyOnly, [this](wgpu::RequestAdapterStatus status, wgpu::Adapter new_adapter, wgpu::StringView message) {
            if (status == wgpu::RequestAdapterStatus::Success)
                webgpu_adapter = new_adapter;
            else
                spdlog::error("WebGPU adapter request failed: {}", message.data);
        });

        Utils::WebGPU::AwaitAsync(webgpu_instance, adapter_future);

        // Device ----------------------------------------
        
        spdlog::trace("Requesting WebGPU device.");

        wgpu::DeviceDescriptor device_descriptor;
        wgpu::Limits& required_limits = *new wgpu::Limits(Utils::WebGPU::CreateRequiredLimits());
        device_descriptor.requiredLimits = &required_limits;

        device_descriptor.SetDeviceLostCallback(wgpu::CallbackMode::AllowProcessEvents, [](const wgpu::Device&, wgpu::DeviceLostReason, wgpu::StringView message) {
            spdlog::error("WebGPU device lost: {}", message.data);
        });
        device_descriptor.SetUncapturedErrorCallback([](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView message) {
            spdlog::error("WebGPU device uncaptured error: {} with error type: ", message.data, ((int)type));
        });

        wgpu::Future device_future = webgpu_adapter.RequestDevice(&device_descriptor, wgpu::CallbackMode::WaitAnyOnly, [this](wgpu::RequestDeviceStatus status, wgpu::Device new_device, wgpu::StringView message) {
          if (status == wgpu::RequestDeviceStatus::Success)
              webgpu_device = new_device;
          else
              spdlog::error("WebGPU device request failed: {}", message.data);
        });

        Utils::WebGPU::AwaitAsync(webgpu_instance, device_future);

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
        render_pass.SetVertexBuffer(0, vertex_buffer, 0, vertex_buffer.GetSize());
        render_pass.Draw(vertex_count, 1, 0, 0);
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
        
        wgpu::ShaderSourceWGSL shader_code({ .nextInChain = nullptr, .code = shader_source.data() });
        
        wgpu::ShaderModuleDescriptor shader_desc{
            .nextInChain = &shader_code,
        };
        
        wgpu::ShaderModule shader_module = webgpu_device.CreateShaderModule(&shader_desc);
        
        wgpu::VertexAttribute attrib[]{
            {.format = wgpu::VertexFormat::Float32x2,
            .offset = 0,
            .shaderLocation = 0},
            {.format = wgpu::VertexFormat::Float32x3,
            .offset = 2 * sizeof(float),
            .shaderLocation = 1},
        };
        
        wgpu::VertexBufferLayout layout{
            .arrayStride = 5 * sizeof(float),
            .attributeCount = 2,
            .attributes = &attrib[0],
        };
        
        wgpu::BlendState blend{
            .color = {
                .operation = wgpu::BlendOperation::Add,
                .srcFactor = wgpu::BlendFactor::SrcAlpha,
                .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
            },
            .alpha = {
                .operation = wgpu::BlendOperation::Add,
                .srcFactor = wgpu::BlendFactor::Zero,
                .dstFactor = wgpu::BlendFactor::One,
            },
        };
        
        wgpu::ColorTargetState color_target{
            .format = webgpu_surface_format,
            .blend = &blend,
            .writeMask = wgpu::ColorWriteMask::All,
        };
        
        wgpu::FragmentState frag{
            .module = shader_module,
            .entryPoint = "fs_main",
            .constantCount = 0,
            .constants = nullptr,
            .targetCount = 1,
            .targets = &color_target,
        };
        
        wgpu::RenderPipelineDescriptor rp_desc{
            .layout = nullptr,
            .vertex = {
                .module = shader_module,
                .entryPoint = "vs_main",
                .constantCount = 0,
                .constants = nullptr,
                .bufferCount = 1,
                .buffers = &layout,
            },
            .primitive = {
                .topology = wgpu::PrimitiveTopology::TriangleList,
                .stripIndexFormat = wgpu::IndexFormat::Undefined,
                .frontFace = wgpu::FrontFace::CCW,
                .cullMode = wgpu::CullMode::None,
            },
            .depthStencil = nullptr,
            .multisample = {
                .count = 1,
                .mask = ~0u,
                .alphaToCoverageEnabled = false,
            },
            .fragment = &frag,
        };
        
        pipeline = webgpu_device.CreateRenderPipeline(&rp_desc);    
    }
    void App::ReadBackBuffer() {

        wgpu::BufferDescriptor buffer_desc;
        buffer_desc.label = "Some GPU-side data buffer";
        buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::CopySrc;
        buffer_desc.size = 16;
        buffer_desc.mappedAtCreation = false;
        wgpu::Buffer buffer1 = webgpu_device.CreateBuffer(&buffer_desc);
        buffer_desc.label = "Some GPU-side data buffer";
        buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead;
        wgpu::Buffer& buffer2 = *new wgpu::Buffer(webgpu_device.CreateBuffer(&buffer_desc));

        std::vector<uint8_t> numbers(16);
        for (uint8_t i = 0; i < 16; ++i) numbers[i] = i;

        webgpu_queue.WriteBuffer(buffer1, 0, numbers.data(), numbers.size());

        wgpu::CommandEncoder encoder = webgpu_device.CreateCommandEncoder(nullptr);

        encoder.CopyBufferToBuffer(buffer1, 0, buffer2, 0, 16);

        wgpu::CommandBuffer cmd = encoder.Finish(nullptr);

        webgpu_queue.Submit(1, &cmd);

        buffer2.MapAsync(wgpu::MapMode::Read, 0, 16, wgpu::CallbackMode::AllowProcessEvents, [](wgpu::MapAsyncStatus status, wgpu::StringView, wgpu::Buffer* buffer) {
            if(status == wgpu::MapAsyncStatus::Success)  {
                spdlog::info("Buffer 2 mapped with status {}", (int)status);
                uint8_t* buffer_data = (uint8_t*)buffer->GetConstMappedRange(0, 16);
                spdlog::info("bufferData = [");
                for (int i = 0; i < 16; ++i) {
                    if (i > 0) spdlog::info(", ");
                    spdlog::info("{}", (int)buffer_data[i]);
                }
                spdlog::info("]");
                buffer->Unmap();
            }
            else
                spdlog::error("Buffer 2 mapped with status {}", (int)status);
        }, &buffer2);

    }
    void App::InitialiseBuffers() {

        std::vector<float> vertex_data = {
            -0.5, -0.5,1.0, 0.0, 0.0,
            +0.5, -0.5,0.0, 1.0, 0.0,
            +0.0, +0.5,0.0, 0.0, 1.0,

            -0.55f, -0.5,1.0, 0.0, 0.0,
            -0.05f, +0.5,0.0, 1.0, 0.0,
            -0.55f, +0.5,0.0, 0.0, 1.0,
        };
        
        vertex_count = static_cast<uint32_t>(vertex_data.size() / 5);

        wgpu::BufferDescriptor buffer_desc;
        buffer_desc.size = vertex_data.size() * sizeof(float);
        buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
        buffer_desc.mappedAtCreation = false;
        vertex_buffer = webgpu_device.CreateBuffer(&buffer_desc);
        
        webgpu_queue.WriteBuffer(vertex_buffer, 0, vertex_data.data(), buffer_desc.size);
    }
}
