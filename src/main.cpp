#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

#include <webgpu/webgpu_glfw.h>
#include "glfw.h"
#include "wgpu.h"

using namespace Chemical;

int main() {

    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("Initialising WebGPU.");

    try {

        struct Core {
            WGPU wgpu;
            GLFW glfw;

            wgpu::Surface surface;
        } core;

        WGPU::print_adapter_info(core.wgpu.adapter);

        // auto& [instance, adapter, device, queue] = wgpu;

        // WGPU::print_adapter_info(adapter);

        // std::array<WGPUCommandBuffer, 3> commands;
        // commands[0] = nul;
        // commands[1] = ;
        // commands[2] = ;
        // wgpuQueueSubmit(queue, commands.size(), commands.data());
        // for (auto cmd : commands) {
        //     wgpuCommandBufferRelease(cmd);
        // }
        // WGPUCommandEncoderDescriptor encoder_desc {
        //     nullptr,
        //     WGPU::string_view("WGPUCommandEncoder")
        // };
        //
        // WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoder_desc);
        // wgpuCommandEncoderInsertDebugMarker(encoder, WGPU::string_view("Debug marker 1"));
        // wgpuCommandEncoderInsertDebugMarker(encoder, WGPU::string_view("Debug marker 2"));
        // wgpuCommandEncoderInsertDebugMarker(encoder, WGPU::string_view("Debug marker 3"));
        //
        // WGPUCommandBufferDescriptor buffer_desc {
        //     nullptr,
        //     WGPU::string_view("WGPUCommandBuffer")
        // };
        // WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &buffer_desc);
        // wgpuQueueSubmit(queue, 1, &command);

    //         wgpuInstanceProcessEvents(instance);
    //         for (int i = 0 ; i < 5 ; ++i) {
    //             spdlog::info("Tick/Poll device...");
    // #ifdef __EMSCRIPTEN__
    //             emscripten_sleep(100);
    // #else
    //             wgpuDeviceTick(device);
    // #endif
    //         }
        // wgpuInstanceProcessEvents(instance);
        // wgpuCommandBufferRelease(command);
        //
    //

#ifdef __EMSCRIPTEN__
        wgpu::EmscriptenSurfaceSourceCanvasHTMLSelector src;
        src.selector = "#canvas";
        wgpu::SurfaceDescriptor desc;
        desc.nextInChain = &src;
        wgpu::Surface surface = wgpu.instance.CreateSurface(&desc);
#else
        core.surface = wgpu::glfw::CreateSurfaceForWindow(core.wgpu.instance, core.glfw.window);
#endif
        wgpu::SurfaceCapabilities capabilities;
        core.surface.GetCapabilities(core.wgpu.adapter, &capabilities);
        wgpu::SurfaceConfiguration config = {};
        config.device = core.wgpu.device;
        config.format = capabilities.formats[0];
        config.width = 1280;
        config.height = 720;
        if(capabilities.presentModeCount == 0) throw std::exception();
        config.presentMode = capabilities.presentModes[0];
        config.alphaMode = wgpu::CompositeAlphaMode::Auto;
        core.surface.Configure(&config);

        auto main_loop_callback = [](void* void_core) {

            Core& core = *reinterpret_cast<Core*>(void_core);

            wgpu::SurfaceTexture surfaceTexture;
            core.surface.GetCurrentTexture(&surfaceTexture);
            // wgpu::CommandBuffer commandBuffer =
            //     wgpu::CreateCommandBuffer(surfaceTexture.texture.CreateView(), pingpong);
            // queue.Submit(1, &commandBuffer);
            // pingpong = (pingpong + 1) % 2;

            glfwPollEvents();
        };

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop_arg(main_loop_callback, &core, 0, true);
#else
        while(!glfwWindowShouldClose(core.glfw.window)) {
            main_loop_callback(&core);
            core.surface.Present();
        }
#endif
    } catch (const std::exception& e) {
        spdlog::critical("Error occurred: {}", e.what());
        return -1;
    }


    
}
