#include <spdlog/spdlog.h>
#include "webgpu/chemical_webgpu.h"

using namespace Chemical;

int main() {

    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("Initialising Chemical::WebGPU.");
    try {
        WebGPU wgpu;
    } catch (const std::runtime_error& e) {
        spdlog::critical("Failed to initialise WebGPU: {}", e.what());
        return -1;
    }
}
