#include <spdlog/spdlog.h>
#include "wgpu.h"

using namespace Chemical;

int main() {

    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("Initialising WebGPU.");
    try {
        auto [instance, adapter, device, queue] = Chemical::WGPU::initialise_webgpu();

        spdlog::trace("Releasing WebGPU instance.");
        wgpuInstanceRelease(instance);
        spdlog::trace("Releasing WebGPU adapter.");
        wgpuAdapterRelease(adapter);
        spdlog::trace("Releasing WebGPU queue.");
        wgpuQueueRelease(queue);
        spdlog::trace("Releasing WebGPU device.");
        wgpuDeviceRelease(device);

    } catch (const std::runtime_error& e) {
        spdlog::critical("Failed to initialise WebGPU: {}", e.what());
        return -1;
    }
}
