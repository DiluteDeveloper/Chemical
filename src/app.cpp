#include "GLFW/glfw3.h"
#include "block/block.h"
#include "glm/ext/matrix_clip_space.hpp"
// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>
#include <cstdint>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_glfw.h>
#include <fstream>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include "app.h"

#include "input.h"
#include "utils/webgpu_utils.h"

namespace Chemical {

    void App::Start() {
        spdlog::set_level(spdlog::level::trace);

#ifdef WSL
        float camera_sensitivity = 0.5;
#else
        float camera_sensitivity = 0.2;
#endif
        App app = {
            .wgpu = SetupWGPU(), 
            .window = SetupGLFW(),
            .buf = InitialiseBuffers(app.wgpu.queue, app.wgpu.device),
            .block = Block::InitialiseBlockBuffer(app.wgpu.device),
            .surface = ConfigureSurface({app.wgpu.instance, app.wgpu.adapter, app.wgpu.device, app.window}),
            .pipeline = SetupRenderPipeline({app.wgpu.device, app.surface.format, app.buf.view_projection_bind_group_layout, app.block}),
            .camera = Camera(0.01, camera_sensitivity),
            .depth = SetupDepthTexture(app.wgpu.device)
        };
        Input::Initialise(app.window);

        Utils::WebGPU::PrintAdapterInfo(app.wgpu.adapter);


#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(ApplicationLoop, &app, 1, true);
#else
        while(!glfwWindowShouldClose(app.window)) {
            ApplicationLoop(&app);
        }
#endif

        glfwDestroyWindow(app.window);
        glfwTerminate();
    }

    void App::ApplicationLoop(void* arg) {
        App& app = *reinterpret_cast<App*>(arg);

        auto[surface_texture, surface_view] = Utils::WebGPU::GetNextSurfaceViewData(app.surface.surface);
        if(!surface_view) return;

        app.RenderPass({app.wgpu.device.CreateCommandEncoder(nullptr), app.wgpu.queue, app.pipeline, surface_view, app.depth.view, app.block, app.buf.vertex_buffer, app.buf.vertex_count, app.buf.view_projection_bind_group});

#ifndef __EMSCRIPTEN__
        app.surface.surface.Present();
#endif
        // wgpu::CommandBuffer commandBuffer =
        //     wgpu::CreateCommandBuffer(surfaceTexture.texture.CreateView(), pingpong);
        // queue.Submit(1, &commandBuffer);
        // pingpong = (pingpong + 1) % 2;

        glfwPollEvents();
        app.camera.Update(app.wgpu.queue, app.buf.view_buffer);
        app.wgpu.instance.ProcessEvents();
    }

    App::SetupWGPUResult App::SetupWGPU() {

        SetupWGPUResult r {
            .instance = wgpu::CreateInstance(nullptr),
            .adapter = {},
            .device = {},
            .queue = {},
        };
        
        // Adapter ----------------------------------------
        
        spdlog::trace("Requesting WebGPU adapter.");

        wgpu::RequestAdapterOptions adapter_opt;
        // adapter_opt.backendType = wgpu::BackendType::Vulkan;
        wgpu::Future adapter_future = r.instance.RequestAdapter(&adapter_opt, wgpu::CallbackMode::WaitAnyOnly, [&r](wgpu::RequestAdapterStatus status, wgpu::Adapter new_adapter, wgpu::StringView message) {
            if (status == wgpu::RequestAdapterStatus::Success)
                r.adapter = new_adapter;
            else
                spdlog::error("WebGPU adapter request failed: {}", message.data);
        });

        Utils::WebGPU::AwaitAsync(r.instance, adapter_future);

        // Device ----------------------------------------
        
        spdlog::trace("Requesting WebGPU device.");

        wgpu::DeviceDescriptor device_descriptor;
        wgpu::Limits& required_limits = *new wgpu::Limits(Utils::WebGPU::CreateRequiredLimits());
        device_descriptor.requiredLimits = &required_limits;

        device_descriptor.SetDeviceLostCallback(wgpu::CallbackMode::AllowProcessEvents, [](const wgpu::Device&, wgpu::DeviceLostReason, wgpu::StringView message) {
            spdlog::error("WebGPU device lost: {}", message.data);
        });
        device_descriptor.SetUncapturedErrorCallback([](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView message) {
            throw std::runtime_error(std::format("WebGPU device uncaptured error: {} with error type: ", message.data, ((int)type)));
        });

        wgpu::Future device_future = r.adapter.RequestDevice(&device_descriptor, wgpu::CallbackMode::WaitAnyOnly, [&r](wgpu::RequestDeviceStatus status, wgpu::Device new_device, wgpu::StringView message) {
          if (status == wgpu::RequestDeviceStatus::Success)
                r.device = new_device;
          else
              spdlog::error("WebGPU device request failed: {}", message.data);
        });

        Utils::WebGPU::AwaitAsync(r.instance, device_future);

        r.queue = r.device.GetQueue();

        return r;

    }

    GLFWwindow* App::SetupGLFW() {

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

        GLFWwindow* glfw_window = glfwCreateWindow(1280, 720, "ChemicalMC_PK", nullptr, nullptr);

        if(!glfw_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }
#ifndef __EMSCRIPTEN__
#ifdef WSL
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#else
        glfwSetInputMode(glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
#endif
        // if(glfwRawMouseMotionSupported())
        //     glfwSetInputMode(glfw_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        return glfw_window;
    }

    App::SurfaceConfig App::ConfigureSurface(const ConfigureSurfaceParam& p) {

#ifdef __EMSCRIPTEN__
        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector src;
        src.selector = "#canvas";
        wgpu::SurfaceDescriptor desc;
        desc.nextInChain = &src;
        App::SurfaceConfig sc = {
            .surface = p.instance.CreateSurface(&desc),
            .format = {}
        };
#else
        App::SurfaceConfig sc = {
            .surface = wgpu::glfw::CreateSurfaceForWindow(p.instance, p.window),
            .format = {}
        };
#endif
        wgpu::SurfaceCapabilities capabilities;
        sc.surface.GetCapabilities(p.adapter, &capabilities);
        wgpu::SurfaceConfiguration config = {};
        config.device = p.device;
        config.format = capabilities.formats[0];
        config.width = 1280;
        config.height = 720;
        if(capabilities.presentModeCount == 0) throw std::exception();
        config.presentMode = capabilities.presentModes[0];
        config.alphaMode = wgpu::CompositeAlphaMode::Auto;
        sc.surface.Configure(&config);
        sc.format = config.format;

        return sc;
    }

    void App::RenderPass(const RenderPassParams& p) {

        wgpu::RenderPassColorAttachment color;
        color.view = p.surface_view;
        color.loadOp = wgpu::LoadOp::Clear;
        color.storeOp = wgpu::StoreOp::Store;
        color.clearValue = wgpu::Color{0.4,0.15,0.2,1.0};

        wgpu::RenderPassDepthStencilAttachment rpdsa {
            .view = p.depth_view,
            .depthLoadOp = wgpu::LoadOp::Clear,
            .depthStoreOp = wgpu::StoreOp::Store,
            .depthClearValue = 1.0
        };
        wgpu::RenderPassDescriptor rp_desc {
            .colorAttachmentCount = 1,
            .colorAttachments = &color,
            .depthStencilAttachment = &rpdsa
        };


        

        wgpu::RenderPassEncoder render_pass = p.encoder.BeginRenderPass(&rp_desc);
    
        render_pass.SetPipeline(p.pipeline);
        render_pass.SetBindGroup(0, p.view_projection_bind_group, 0, nullptr);
        render_pass.SetBindGroup(1, p.block.group, 0, nullptr);
        render_pass.Draw(p.block.vertex_count, 1, 0, 0);
        render_pass.End();

        wgpu::CommandBuffer cmd = p.encoder.Finish(nullptr);

        p.queue.Submit(1, &cmd);
    }

    wgpu::RenderPipeline App::SetupRenderPipeline(const SetupRenderPipelineParam& p) {
        std::ifstream shader_file("res/shaders/main.wgsl");
        if (!shader_file.is_open())
            throw std::runtime_error("Failed to read file: res/shaders/main.wgsl");
        

        std::string shader_source((std::istreambuf_iterator<char>(shader_file)),
            std::istreambuf_iterator<char>());
        
        wgpu::ShaderSourceWGSL shader_code({ .nextInChain = nullptr, .code = shader_source.data() });
        
        wgpu::ShaderModuleDescriptor shader_desc{
            .nextInChain = &shader_code,
        };
        
        wgpu::ShaderModule shader_module = p.device.CreateShaderModule(&shader_desc);
        
        // wgpu::VertexAttribute attrib[]{
        //     {.format = wgpu::VertexFormat::Float32x3,
        //     .offset = 0,
        //     .shaderLocation = 0},
        //     {.format = wgpu::VertexFormat::Float32x3,
        //     .offset = 3 * sizeof(float),
        //     .shaderLocation = 1},
        // };
        //
        // wgpu::VertexBufferLayout layout{
        //     .arrayStride = 6 * sizeof(float),
        //     .attributeCount = 2,
        //     .attributes = &attrib[0],
        // };
        
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
            .format = p.surface_format,
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

        wgpu::BindGroupLayout layouts[] = {
            p.view_projection_bind_group_layout, p.block.layout
        };

        wgpu::PipelineLayoutDescriptor pld {
            .bindGroupLayoutCount = 2,
            .bindGroupLayouts = layouts,
        };
        
        wgpu::DepthStencilState dss {
            .format = wgpu::TextureFormat::Depth32Float,
            .depthWriteEnabled = wgpu::OptionalBool::True,
            .depthCompare = wgpu::CompareFunction::Less
        };
        wgpu::RenderPipelineDescriptor rp_desc{
            .layout = p.device.CreatePipelineLayout(&pld),
            .vertex = {
                .module = shader_module,
                .entryPoint = "vs_main",
                .constantCount = 0,
                .constants = nullptr,
                .bufferCount = 0,
                .buffers = nullptr,
            },
            .primitive = {
                .topology = wgpu::PrimitiveTopology::TriangleList,
                .stripIndexFormat = wgpu::IndexFormat::Undefined,
                .frontFace = wgpu::FrontFace::CCW,
                .cullMode = wgpu::CullMode::Back,
            },
            .depthStencil = &dss,
            .multisample = {
                .count = 1,
                .mask = ~0u,
                .alphaToCoverageEnabled = false,
            },
            .fragment = &frag

        };
        
        return p.device.CreateRenderPipeline(&rp_desc);    
    }

    // void App::ReadBackBuffer() {
    //
    //     wgpu::BufferDescriptor buffer_desc;
    //     buffer_desc.label = "Some GPU-side data buffer";
    //     buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::CopySrc;
    //     buffer_desc.size = 16;
    //     buffer_desc.mappedAtCreation = false;
    //     wgpu::Buffer buffer1 = webgpu_device.CreateBuffer(&buffer_desc);
    //     buffer_desc.label = "Some GPU-side data buffer";
    //     buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead;
    //     wgpu::Buffer& buffer2 = *new wgpu::Buffer(webgpu_device.CreateBuffer(&buffer_desc));
    //
    //     std::vector<uint8_t> numbers(16);
    //     for (uint8_t i = 0; i < 16; ++i) numbers[i] = i;
    //
    //     webgpu_queue.WriteBuffer(buffer1, 0, numbers.data(), numbers.size());
    //
    //     wgpu::CommandEncoder encoder = webgpu_device.CreateCommandEncoder(nullptr);
    //
    //     encoder.CopyBufferToBuffer(buffer1, 0, buffer2, 0, 16);
    //
    //     wgpu::CommandBuffer cmd = encoder.Finish(nullptr);
    //
    //     webgpu_queue.Submit(1, &cmd);
    //
    //     buffer2.MapAsync(wgpu::MapMode::Read, 0, 16, wgpu::CallbackMode::AllowProcessEvents, [](wgpu::MapAsyncStatus status, wgpu::StringView, wgpu::Buffer* buffer) {
    //         if(status == wgpu::MapAsyncStatus::Success)  {
    //             spdlog::info("Buffer 2 mapped with status {}", (int)status);
    //             uint8_t* buffer_data = (uint8_t*)buffer->GetConstMappedRange(0, 16);
    //             spdlog::info("bufferData = [");
    //             for (int i = 0; i < 16; ++i) {
    //                 if (i > 0) spdlog::info(", ");
    //                 spdlog::info("{}", (int)buffer_data[i]);
    //             }
    //             spdlog::info("]");
    //             buffer->Unmap();
    //         }
    //         else
    //             spdlog::error("Buffer 2 mapped with status {}", (int)status);
    //     }, &buffer2);
    //
    // }

    App::BufferInit App::InitialiseBuffers(wgpu::Queue queue, wgpu::Device device) {

        std::vector<float> vertex_data = {
            -0.5, -0.5,1.0,1.0, 0.0, 0.0,
            +0.5, -0.5,1.0,0.0, 1.0, 0.0,
            +0.0, +0.5,1.0, 0.0, 0.0, 1.0,

            1.9, -0.5,3.0,1.0, 0.0, 0.0,
            +2.7, -0.5,3.0,0.0, 1.0, 0.0,
            +2.2, +0.5,3.0, 0.0, 0.0, 1.0,

            -1.9, -0.5,2.0,1.0, 0.0, 0.0,
            +0.0, -0.5,2.0,0.0, 1.0, 0.0,
            +1.0, +0.5,2.0, 0.0, 0.0, 1.0,
        };

        BufferInit out;
        
        out.vertex_count = static_cast<uint32_t>(vertex_data.size() / 6);

        wgpu::BufferDescriptor buffer_desc;
        buffer_desc.size = vertex_data.size() * sizeof(float);
        buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Vertex;
        buffer_desc.mappedAtCreation = false;
        out.vertex_buffer = device.CreateBuffer(&buffer_desc);
        
        queue.WriteBuffer(out.vertex_buffer, 0, vertex_data.data(), buffer_desc.size);

        buffer_desc.size = sizeof(float) * 4 * 4;
        buffer_desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform;
        buffer_desc.mappedAtCreation = true;

        out.view_buffer = device.CreateBuffer(&buffer_desc);

        glm::mat4& view_matrix = *(glm::mat4*)out.view_buffer.GetMappedRange();
        view_matrix = glm::mat4(1.0f);

        out.view_buffer.Unmap();

        out.projection_buffer = device.CreateBuffer(&buffer_desc);

        glm::mat4& projection_matrix = *(glm::mat4*)out.projection_buffer.GetMappedRange();
        projection_matrix = GenerateProjectionMatrix(1280, 720);

        out.projection_buffer.Unmap();

        wgpu::BindGroupEntry view_bge[] {
            {
                .binding = 0,
                .buffer = out.view_buffer,
                .offset = 0,
                .size = sizeof(float) * 4 * 4
            },
            {
                .binding = 1,
                .buffer = out.projection_buffer,
                .offset = 0,
                .size = sizeof(float) * 4 * 4
            }

        };
        wgpu::BindGroupLayoutEntry view_bgle[] {
            {
                .binding = 0,
                .visibility = wgpu::ShaderStage::Vertex,
                .buffer = { nullptr, wgpu::BufferBindingType::Uniform, false, 0}
            },
            {
                .binding = 1,
                .visibility = wgpu::ShaderStage::Vertex,
                .buffer = { nullptr, wgpu::BufferBindingType::Uniform, false, 0}
            }

        };
        wgpu::BindGroupLayoutDescriptor bgld {
            .entryCount = 2,
            .entries = &view_bgle[0]
        };
        out.view_projection_bind_group_layout = device.CreateBindGroupLayout(&bgld);
        wgpu::BindGroupDescriptor view_projection_bind_group_descriptor {
            .layout = out.view_projection_bind_group_layout,
            .entryCount = 2,
            .entries = &view_bge[0]
        };
        out.view_projection_bind_group = device.CreateBindGroup(&view_projection_bind_group_descriptor);

        return out;
    }

    glm::fmat4 App::GenerateProjectionMatrix(float width, float height, 
                float fov, float near, float far) {
        return glm::perspectiveFovLH(glm::radians(fov), width, height, near, far);
        // glm::vec4 test = {1.0, 0.0, 5.0, 1.0};
        // glm::vec4 t2 = t * test;
        // spdlog::info("{}", glm::to_string(t));
        // return t;
    }

    App::Depth App::SetupDepthTexture(wgpu::Device device) {
        wgpu::TextureDescriptor desc = {
            .usage = wgpu::TextureUsage::RenderAttachment,
            .dimension = wgpu::TextureDimension::e2D,
            .size = wgpu::Extent3D(1280, 720, 1),
            .format = wgpu::TextureFormat::Depth32Float
        };
        auto tex = device.CreateTexture(&desc);
        auto view = tex.CreateView();
        return Depth { tex, view };
    }

}
