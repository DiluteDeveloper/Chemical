#include "webgpu.h"

#include <spdlog/spdlog.h>


namespace Chemical {
    
    WebGPU* WebGPU::initialise() {

        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;
        
        spdlog::trace("Creating WebGPU instance.");

#ifdef WEBGPU_BACKEND_EMSCRIPTEN
        WGPUInstance wgpu_instance = wgpuCreateInstance(nullptr);
#else
        WGPUInstance wgpu_instance = wgpuCreateInstance(&desc);
#endif

        if(!wgpu_instance) {
            spdlog::critical("Failed to create WebGPU instance.");
            return nullptr;
        }

        WebGPU* wgpu = new WebGPU();
        wgpu->wgpu_instance = wgpu_instance;
        return wgpu;
    }

    void WebGPU::shutdown(WebGPU* wgpu) {

        spdlog::trace("Releasing WebGPU instance.");
        // Drop down reference count and release if 0
        wgpuInstanceRelease(wgpu->wgpu_instance);

        spdlog::trace("Releasing Chemical::WebGPU.");
        delete wgpu;
    }
}
