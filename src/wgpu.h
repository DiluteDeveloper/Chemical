#pragma once

#ifdef __EMSCRIPTEN__
    #include <webgpu/webgpu_cpp.h>
#else
    #include <webgpu/webgpu.h>
#endif

#include <unordered_map>
#include <string_view>

namespace Chemical {

    class WGPU {
    public:

        static std::tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue> initialise_webgpu();
        static void terminate_webgpu(std::tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue>& wgpu);

        static void print_adapter_info(WGPUAdapter& adapter);
        static WGPUStringView 
            string_view(const std::string_view& string);
    private:

        static void request_adapter_async(WGPUInstance instance, WGPUAdapter& out_adapter, bool& out_is_adapter_processed, const WGPURequestAdapterOptions* options);

        static void request_device_async(WGPUAdapter& adapter, WGPUDevice& out_device, bool& out_is_device_processed, const WGPUDeviceDescriptor* descriptor);

        static std::unordered_map<int, 
            const char*>& get_feature_name_map();
        static std::unordered_map<int, 
            const char*>& get_backend_type_map();
        static std::unordered_map<int, 
            const char*>& get_adapter_type_map();


    };
}

