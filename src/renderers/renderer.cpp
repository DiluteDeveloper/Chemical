#include "renderer.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "character_controller.h"
#include "renderers/static_block_renderer.h"
#include "webgpu/webgpu_cpp.h"
#include <spdlog/spdlog.h>
#include <webgpu/webgpu_glfw.h>
#include <imgui.h>
#include <imgui_impl_wgpu.h>
#include <imgui_impl_glfw.h>
#include <entt/entity/registry.hpp>

namespace Chemical {

    Renderer::Renderer(GLFWwindow* window, entt::registry& registry) : registry(&registry) {
        InitialiseWebGPU(window);
        auto [_view_buffer, _camera_group, _layout] = InitialiseViewProjection();
        camera_group = _camera_group;
        view_buffer = _view_buffer;
        view_projection_layout = _layout;

        InitialiseImgui(window);

        auto [surface_depth_texture, surface_depth_texture_view] = CreateDepthTexture();
        InitialisePrimaryRenderPass(surface_depth_texture_view);
        InitialiseImguiRenderPass();

        registry.ctx().get<CharacterController>().SetViewMatrixUpdateCallback([this](const glm::mat4& view_matrix) {
            queue.WriteBuffer(view_buffer, 0, &view_matrix[0][0], sizeof(glm::mat4));
        });
    }
    Renderer::~Renderer() {
        ImGui_ImplWGPU_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void Renderer::Render() {
        ImGui_ImplWGPU_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        auto& controller = registry->ctx().get<CharacterController>();
        ImGui::Begin("Player Configuration");

        ImGui::InputFloat("Set Speed", &controller.base_speed, 0, 5, "%.5f");
        ImGui::InputFloat("Set Bhop Speed Increase Rate", &controller.bhop_speed_increase_rate, 0, 5, "%.5f");
        ImGui::InputFloat("Set Bhop Speed Decrease Rate", &controller.bhop_speed_decrease_rate, 0, 5, "%.5f");
        ImGui::Text("Bhop speed: %.2f", controller.bhop_speed);
        ImGui::InputFloat("Set Jump Force", &controller.jump_force, 0, 5, "%.5f");
        ImGui::InputFloat("Set Gravity", &controller.gravity, 0, 5, "%.5f");
        ImGui::InputFloat("Set Sensitivity", &controller.sensitivity, 0, 5, "%.5f");
        // ImGui::InputFloat("Set Jump Force", &jump_force, 0, 50, "%.5f");
        ImGui::InputFloat3("Set Controller Friction", &controller.controller_friction[0], 0, 1);
        ImGui::InputFloat3("Set Freecam Friction", &controller.freecam_friction[0], 0, 1);
        if(ImGui::Button("Toggle Freecam")) {
            controller.freecam_enabled = !controller.freecam_enabled;
        }
        ImGui::End();

        ImGui::Render();

        auto[surface_texture, surface_view] = GetNextSurfaceViewData();
        if(!surface_view) return;

        wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
        ExecutePrimaryRenderPass(encoder, surface_view);
        ExecuteImguiRenderPass(encoder, surface_view);

        wgpu::CommandBuffer cmd = encoder.Finish(nullptr);
        queue.Submit(1, &cmd);

#ifndef __EMSCRIPTEN__
        surface.Present();
        device.Tick();
#endif

        instance.ProcessEvents();
    }

    void Renderer::ExecutePrimaryRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view) {
        primary_pass.color.view = surface_view;
        wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&primary_pass.descriptor);
        pass.SetBindGroup(0, camera_group, 0, nullptr);
        registry->ctx().get<StaticBlockRenderer>().Render(pass);
        pass.End();
    }

    void Renderer::ExecuteImguiRenderPass(wgpu::CommandEncoder encoder, wgpu::TextureView surface_view) {
        imgui_pass.color.view = surface_view;
        wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&imgui_pass.descriptor);
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass.Get());
        pass.End();
    }

    void Renderer::InitialisePrimaryRenderPass(wgpu::TextureView depth_texture_view) {
        primary_pass.color = {
            .loadOp = wgpu::LoadOp::Clear,
            .storeOp = wgpu::StoreOp::Store,
            .clearValue = wgpu::Color{0.2,0.2,0.3,1.0}
        };
        primary_pass.depth = {
            .view = depth_texture_view,
            .depthLoadOp = wgpu::LoadOp::Clear,
            .depthStoreOp = wgpu::StoreOp::Store,
            .depthClearValue = 1.0
        };
        primary_pass.descriptor = {
            .label = "PrimaryRenderPass",
            .colorAttachmentCount = 1,
            .colorAttachments = &primary_pass.color,
            .depthStencilAttachment = &primary_pass.depth
        };
    }

    void Renderer::InitialiseImguiRenderPass() {

        imgui_pass.color = {
            .loadOp = wgpu::LoadOp::Load,
            .storeOp = wgpu::StoreOp::Store,
            .clearValue = wgpu::Color{0.0,0.0,0.0,0.0}
        };
        imgui_pass.descriptor = {
            .colorAttachmentCount = 1,
            .colorAttachments = &imgui_pass.color,
        };
    }

    void Renderer::AwaitAsync(const wgpu::Future& future) {
        while(true) {
#ifdef __EMSCRIPTEN__
            emscripten_sleep(10); 
#endif
            if(wgpu::WaitStatus s = instance.WaitAny(future, 0); s == wgpu::WaitStatus::Success)
                return;
        }
    }

    void Renderer::InitialiseWebGPU(GLFWwindow* window) {

        instance = wgpu::CreateInstance(nullptr),
        
        // Adapter ----------------------------------------
        
        spdlog::trace("Requesting WebGPU adapter.");

        wgpu::RequestAdapterOptions adapter_opt;
        // adapter_opt.backendType = wgpu::BackendType::Vulkan;
        wgpu::Future adapter_future = instance.RequestAdapter(&adapter_opt, wgpu::CallbackMode::WaitAnyOnly, [this](wgpu::RequestAdapterStatus status, wgpu::Adapter new_adapter, wgpu::StringView message) {
            if (status == wgpu::RequestAdapterStatus::Success)
                adapter = new_adapter;
            else
                spdlog::error("WebGPU adapter request failed: {}", message.data);
        });

        AwaitAsync(adapter_future);

        // Device ----------------------------------------
        
        spdlog::trace("Requesting WebGPU device.");

        wgpu::DeviceDescriptor device_descriptor;
        wgpu::Limits required_limits = {
            // .maxVertexBuffers = 1,
            // .maxBufferSize = 5000 * sizeof(float),
            // .maxVertexAttributes = 2,
            // .maxVertexBufferArrayStride = 6 * sizeof(float),
            // .maxInterStageShaderVariables = 3
        };
        device_descriptor.requiredLimits = &required_limits;

        device_descriptor.SetDeviceLostCallback(wgpu::CallbackMode::AllowProcessEvents, [](const wgpu::Device&, wgpu::DeviceLostReason, wgpu::StringView message) { throw std::runtime_error(std::format("WebGPU device lost: {}", message.data)); });

        device_descriptor.SetUncapturedErrorCallback([](const wgpu::Device&, wgpu::ErrorType type, wgpu::StringView message) {
            throw std::runtime_error(std::format("WebGPU device uncaptured error: {} with error type: ", message.data, ((int)type)));
        });

        wgpu::Future device_future = adapter.RequestDevice(&device_descriptor, wgpu::CallbackMode::WaitAnyOnly, [this](wgpu::RequestDeviceStatus status, wgpu::Device new_device, wgpu::StringView message) {
          if (status == wgpu::RequestDeviceStatus::Success)
                device = new_device;
          else
              throw std::runtime_error(std::format("WebGPU device request failed: {}", message.data));
        });

        AwaitAsync(device_future);

        queue = device.GetQueue();
        
#ifdef __EMSCRIPTEN__
        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector src;
        src.selector = "#canvas";
        wgpu::SurfaceDescriptor desc;
        desc.nextInChain = &src;
        surface = wgpu.instance.CreateSurface(&desc),
#else
        surface = wgpu::glfw::CreateSurfaceForWindow(instance, window);
#endif
        wgpu::SurfaceCapabilities capabilities;
        surface.GetCapabilities(adapter, &capabilities);
        if(capabilities.presentModeCount == 0) 
            throw std::runtime_error("WebGPU surface has no present modes!");

        wgpu::SurfaceConfiguration config = {
            .device = device,
            .format = capabilities.formats[0],
            .width = 1920,
            .height = 1080,
            .alphaMode = wgpu::CompositeAlphaMode::Auto,
            .presentMode = capabilities.presentModes[0]
        };
        surface.Configure(&config);
        surface_format = config.format;
    }

    std::tuple<wgpu::Buffer, wgpu::BindGroup, wgpu::BindGroupLayout> Renderer::InitialiseViewProjection() {
        
        wgpu::BufferDescriptor bd = {
            .usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::Uniform,
            .size = sizeof(float) * 4 * 4,
            .mappedAtCreation = true
        };
        wgpu::Buffer view_buffer = device.CreateBuffer(&bd);

        glm::mat4 matrix = glm::mat4(1.0f);
        view_buffer.WriteMappedRange(0, &matrix, bd.size);
        view_buffer.Unmap();

        wgpu::Buffer projection_buffer = device.CreateBuffer(&bd);

        matrix = glm::perspectiveFovLH(glm::radians(90.0f), 1920.0f, 1080.0f, 0.01f, 1000.0f);
        projection_buffer.WriteMappedRange(0, &matrix, bd.size);
        projection_buffer.Unmap();

        wgpu::BindGroupEntry view_bge[] {
            {
                .binding = 0,
                .buffer = view_buffer,
                .offset = 0,
                .size = bd.size
            },
            {
                .binding = 1,
                .buffer = projection_buffer,
                .offset = 0,
                .size = bd.size
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
        wgpu::BindGroupLayout layout = device.CreateBindGroupLayout(&bgld);
        wgpu::BindGroupDescriptor bind_group_descriptor {
            .layout = layout,
            .entryCount = 2,
            .entries = &view_bge[0]
        };
        return {view_buffer, device.CreateBindGroup(&bind_group_descriptor), layout};
    }

    std::tuple<wgpu::SurfaceTexture, wgpu::TextureView> Renderer::GetNextSurfaceViewData() {
        wgpu::SurfaceTexture surface_texture;
        surface.GetCurrentTexture(&surface_texture);
        if(surface_texture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal) {
            spdlog::warn("Surface texture status is not success optimal");
            return std::make_tuple(surface_texture, nullptr);
        }

        wgpu::TextureViewDescriptor view_descriptor;
        view_descriptor.format = surface_texture.texture.GetFormat();
        view_descriptor.dimension = wgpu::TextureViewDimension::e2D;
        view_descriptor.mipLevelCount = 1;
        view_descriptor.arrayLayerCount = 1;
        view_descriptor.aspect = wgpu::TextureAspect::All;
        wgpu::TextureView target_view = surface_texture.texture.CreateView(&view_descriptor);
        if(!target_view) {
            spdlog::warn("Surface texture view failed!");
            return std::make_tuple(surface_texture, nullptr);
        }
        
        return std::make_tuple(surface_texture, target_view);
    }

    std::tuple<wgpu::Texture, wgpu::TextureView> Renderer::CreateDepthTexture() {
        wgpu::TextureDescriptor desc = {
            .usage = wgpu::TextureUsage::RenderAttachment,
            .dimension = wgpu::TextureDimension::e2D,
            .size = wgpu::Extent3D(1920, 1080, 1),
            .format = wgpu::TextureFormat::Depth32Float
        };
        auto tex = device.CreateTexture(&desc);
        auto view = tex.CreateView();
        return {tex, view};
    }

    void Renderer::InitialiseImgui(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOther(window, true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
        ImGui_ImplWGPU_InitInfo init_info;
        init_info.Device = device.Get();
        init_info.NumFramesInFlight = 3;
        init_info.RenderTargetFormat = (WGPUTextureFormat)surface_format;
        init_info.DepthStencilFormat = WGPUTextureFormat_Undefined;
        ImGui_ImplWGPU_Init(&init_info);
#ifdef __EMSCRIPTEN__
        io.IniFilename = nullptr;
#endif
    }

}
