#pragma once

#include "webgpu_header.h"

namespace Chemical {
    
    class WebGPU {

    public:

        // Returns nullptr on failure
        static WebGPU* initialise();
        static void shutdown(WebGPU* wgpu);

    private:
        WGPUAdapter wgpu_adapter;

        static void request_adapter_async(WGPUInstance instance, WGPUAdapter& out_adapter, bool& out_is_adapter_processed, const WGPURequestAdapterOptions* options);
        // WGPUAdapter request_adapter_sync(WGPURequestAdapterStatus status,
        //         WGPUAdapter adapter, WGPUStringView message, 
        //         void* userdata1, void* _userdata2);
    };
}

