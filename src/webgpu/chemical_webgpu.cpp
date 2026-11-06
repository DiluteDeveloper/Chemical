#include "chemical_webgpu.h"

#include <cassert>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace Chemical {
    
    WebGPU* WebGPU::initialise() {

        spdlog::trace("Creating WebGPU instance.");

        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;

#ifdef __EMSCRIPTEN__
        spdlog::trace("Using Emscripten WebGPU backend.");
        WGPUInstance wgpu_instance = wgpuCreateInstance(nullptr);
#else
        WGPUInstance wgpu_instance = wgpuCreateInstance(&desc);
#endif

        if(!wgpu_instance) {
            spdlog::error("Failed to create WebGPU instance.");
            return nullptr;
        }

        struct AsyncObjectRequests {
            WGPUAdapter adapter = nullptr;
            bool is_adapter_processed = false;
        } requests;

        spdlog::trace("Requesting WebGPU adapter.");

        WGPURequestAdapterOptions adapter_options = {};
        adapter_options.nextInChain = nullptr;

        request_adapter_async(wgpu_instance, requests.adapter, requests.is_adapter_processed, &adapter_options);

#ifdef __EMSCRIPTEN__
        while(!requests.is_adapter_processed) {
            emscripten_sleep(100); 
            wgpuInstanceProcessEvents(wgpu_instance);
        }
#else
        wgpuInstanceProcessEvents(wgpu_instance);
#endif

        if(!requests.adapter) {
            spdlog::error("Failed to request WebGPU adapter.");
            return nullptr;
        }

        spdlog::trace("Releasing WebGPU instance.");

        // Drop down reference count and release if 0 (Referenced by adapter)
        wgpuInstanceRelease(wgpu_instance);

        WebGPU* wgpu = new WebGPU();
        wgpu->wgpu_adapter = requests.adapter;
        return wgpu;
    }

    void WebGPU::shutdown(WebGPU* wgpu) {

        spdlog::trace("Releasing WebGPU adapter.");
        wgpuAdapterRelease(wgpu->wgpu_adapter);

        spdlog::trace("Releasing Chemical::WebGPU.");
        delete wgpu;
    }

    void WebGPU::request_adapter_async(WGPUInstance instance, WGPUAdapter& out_adapter, bool& out_is_adapter_processed, const WGPURequestAdapterOptions* options) {

        auto on_adapter_request_ended = [](WGPURequestAdapterStatus status,
                WGPUAdapter adapter, WGPUStringView message, 
                void* out_adapter, void* out_is_adapter_processed) {
            if (status == WGPURequestAdapterStatus_Success)
                *reinterpret_cast<WGPUAdapter*>(out_adapter) = adapter;
            else
                spdlog::error("WebGPU adapter request failed: {}", message.data);
            *reinterpret_cast<bool*>(out_is_adapter_processed) = true;

        };

        wgpuInstanceRequestAdapter(
            instance, options, WGPURequestAdapterCallbackInfo{
                     nullptr,    
                     WGPUCallbackMode::WGPUCallbackMode_AllowProcessEvents,
                     on_adapter_request_ended,
                     &out_adapter,
                     &out_is_adapter_processed
            }
        );
    }
}
