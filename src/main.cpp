#include <spdlog/spdlog.h>
#include "webgpu.h"

using namespace Chemical;

int main() {

    spdlog::set_level(spdlog::level::trace);

    spdlog::trace("Initialising Chemical::WebGPU.");
    WebGPU* wgpu = WebGPU::initialise();

    if(!wgpu) {
        spdlog::critical("Failed to initialise Chemical::WebGPU.");
        return -1;
    }

    WebGPU::shutdown(wgpu);
}
