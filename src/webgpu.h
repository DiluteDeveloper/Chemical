#pragma once

#include <webgpu/webgpu.h>
#include <memory>

namespace Chemical {
    
    class WebGPU {

    public:

        // Returns nullptr on failure
        static WebGPU* initialise();
        static void shutdown(WebGPU* wgpu);

    private:
        WGPUInstance wgpu_instance;
    };
}

