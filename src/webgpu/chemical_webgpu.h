#pragma once

#include <unordered_map>
#include <memory>

#include "webgpu_header.h"

namespace Chemical {
    
    class WebGPU {

    public:

        // Throws std::exception on failure
        WebGPU();
        ~WebGPU();
    private:

        WGPUAdapter adapter;

        static void request_adapter_async(WGPUInstance instance, WGPUAdapter& out_adapter, bool& out_is_adapter_processed, const WGPURequestAdapterOptions* options);


        static void print_adapter_info(WGPUAdapter& adapter);
        static std::unique_ptr<std::unordered_map<int, const char*>> feature_names;
        static std::unique_ptr<std::unordered_map<int, const char*>> backend_types;
        static std::unique_ptr<std::unordered_map<int, const char*>> adapter_types;
        static void set_feature_names();
        static void set_backend_types();
        static void set_adapter_types();
    };
}

