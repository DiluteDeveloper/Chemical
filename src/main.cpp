#include <spdlog/spdlog.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif
#include "glfw.h"
#include "wgpu.h"

using namespace Chemical;

int main() {

    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("Initialising WebGPU.");

    std::tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue> wgpu;
    try {
        wgpu = WGPU::initialise_webgpu();
    } catch (const std::runtime_error& e) {
        spdlog::critical("Failed to initialise WebGPU: {}", e.what());
        return -1;
    }
    auto& [instance, adapter, device, queue] = wgpu;

    WGPU::print_adapter_info(adapter);

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
    wgpuInstanceProcessEvents(instance);
    // wgpuCommandBufferRelease(command);
    //

    GLFWwindow* window;
    try {
        window = GLFW::initialise_glfw();

    } catch (const std::runtime_error& e) {
        spdlog::critical("Failed to initialise GLFW: {}", e.what());
        return -1;
    }

    struct MainLoopData {
        WGPUDevice device;
        WGPUQueue queue;
        GLFWwindow* window;
    } main_loop_data { device, queue, window };

    auto main_loop_callback = [](void* main_loop_data) {
        glfwPollEvents();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop_callback, &main_loop_data, 0, true);
#else
    while(!glfwWindowShouldClose(window)) {
        main_loop_callback(&main_loop_data);
    }
#endif


    GLFW::terminate_glfw(window);
    WGPU::terminate_webgpu(wgpu);

    
}
