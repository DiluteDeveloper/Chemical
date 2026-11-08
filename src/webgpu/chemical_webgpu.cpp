#include "chemical_webgpu.h"

#include <cassert>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace Chemical {
    
    WebGPU::WebGPU() {

        spdlog::trace("Creating WebGPU instance.");

        WGPUInstanceDescriptor desc = {};
        desc.nextInChain = nullptr;

#ifdef __EMSCRIPTEN__
        spdlog::trace("Using Emscripten WebGPU backend.");
        WGPUInstance wgpu_instance = wgpuCreateInstance(nullptr);
#else
        WGPUInstance wgpu_instance = wgpuCreateInstance(&desc);
#endif

        if(!wgpu_instance)
            throw std::runtime_error("Failed to create WebGPU instance.");

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

        if(!requests.adapter)
            throw std::runtime_error("Failed to request WebGPU adapter.");

        adapter = requests.adapter;
        print_adapter_info(adapter);

        spdlog::trace("Releasing WebGPU instance.");

        // Drop down reference count and release if 0 (Referenced by adapter so released with adapter now)
        wgpuInstanceRelease(wgpu_instance);
    }

    WebGPU::~WebGPU() {

        spdlog::trace("Releasing WebGPU adapter.");
        wgpuAdapterRelease(adapter);
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
    void WebGPU::print_adapter_info(WGPUAdapter& adapter) {

        set_backend_types();
        set_adapter_types();

        WGPUAdapterInfo info;
        info.nextInChain = nullptr;

        wgpuAdapterGetInfo(adapter, &info);

        spdlog::trace("WebGPU adapter information {");

		spdlog::trace("\tvendor: {},", info.vendor.data);
		spdlog::trace("\tarchitecture: {},", info.architecture.data);
		spdlog::trace("\tdevice: {},", info.device.data);
		spdlog::trace("\tdescription: {},", info.description.data);

        // Just let it crash honestly its so rare
		spdlog::trace("\tbackendType: {},", backend_types->at(info.backendType));
		spdlog::trace("\tadapterType: {},", adapter_types->at(info.adapterType));
		spdlog::trace("\tvendorID: {},", info.vendorID);
		spdlog::trace("\tdeviceID: {},", info.deviceID);
		spdlog::trace("\tsubgroupMinSize: {},", info.subgroupMinSize);
		spdlog::trace("\tsubgroupMaxSize: {},", info.subgroupMaxSize);

        spdlog::trace("}");

        wgpuAdapterInfoFreeMembers(info);

        set_feature_names();

        // This segfaults when initialised to nullptr?
        // Im not sure im using this API correctly or deleting it correctly
        WGPUSupportedFeatures* features = new WGPUSupportedFeatures();
        wgpuAdapterGetFeatures(adapter, features);

        spdlog::trace("WebGPU adapter supported features {");

        for(size_t i = 0; i < features->featureCount; i++) {
            int feature = *(features->features + i);
            if (auto search = feature_names->find(feature); search != feature_names->end())
                spdlog::trace("\t{},", search->second);
            else
                spdlog::warn("\tWebGPU adapter feature {0:x} does not appear to have a reflected name, Chemical may not support this feature.", feature);
            
        }

        spdlog::trace("}");

        wgpuSupportedFeaturesFreeMembers(*features);
        delete features;

        // Does not need to be freed supposedly
        WGPULimits limits;
        limits.nextInChain = nullptr;

        wgpuAdapterGetLimits(adapter, &limits);

        spdlog::trace("WebGPU adapter limits {");


		spdlog::trace("\tmaxTextureDimension1D: {}", limits.maxTextureDimension1D);
		spdlog::trace("\tmaxTextureDimension2D: {}", limits.maxTextureDimension2D);
		spdlog::trace("\tmaxTextureDimension3D: {}", limits.maxTextureDimension3D);
		spdlog::trace("\tmaxTextureArrayLayers: {}", limits.maxTextureArrayLayers);
		spdlog::trace("\tmaxBindGroups: {}", limits.maxBindGroups);
		spdlog::trace("\tmaxBindGroupsPlusVertexBuffers: {}", limits.maxBindGroupsPlusVertexBuffers);
		spdlog::trace("\tmaxBindingsPerBindGroup: {}", limits.maxBindingsPerBindGroup);
		spdlog::trace("\tmaxDynamicUniformBuffersPerPipelineLayout: {}", limits.maxDynamicUniformBuffersPerPipelineLayout);
		spdlog::trace("\tmaxDynamicStorageBuffersPerPipelineLayout: {}", limits.maxDynamicStorageBuffersPerPipelineLayout);
		spdlog::trace("\tmaxSampledTexturesPerShaderStage: {}", limits.maxSampledTexturesPerShaderStage);
		spdlog::trace("\tmaxSamplersPerShaderStage: {}", limits.maxSamplersPerShaderStage);
		spdlog::trace("\tmaxStorageBuffersPerShaderStage: {}", limits.maxStorageBuffersPerShaderStage);
		spdlog::trace("\tmaxStorageTexturesPerShaderStage: {}", limits.maxStorageTexturesPerShaderStage);
		spdlog::trace("\tmaxUniformBuffersPerShaderStage: {}", limits.maxUniformBuffersPerShaderStage);
		spdlog::trace("\tmaxUniformBufferBindingSize: {}", limits.maxUniformBufferBindingSize);
		spdlog::trace("\tmaxStorageBufferBindingSize: {}", limits.maxStorageBufferBindingSize);
		spdlog::trace("\tminUniformBufferOffsetAlignment: {}", limits.minUniformBufferOffsetAlignment);
		spdlog::trace("\tminStorageBufferOffsetAlignment: {}", limits.minStorageBufferOffsetAlignment);
		spdlog::trace("\tmaxVertexBuffers: {}", limits.maxVertexBuffers);
		spdlog::trace("\tmaxBufferSize: {}", limits.maxBufferSize);
		spdlog::trace("\tmaxVertexAttributes: {}", limits.maxVertexAttributes);
		spdlog::trace("\tmaxVertexBufferArrayStride: {}", limits.maxVertexBufferArrayStride);
		spdlog::trace("\tmaxInterStageShaderVariables: {}", limits.maxInterStageShaderVariables);
		spdlog::trace("\tmaxColorAttachments: {}", limits.maxColorAttachments);
		spdlog::trace("\tmaxColorAttachmentBytesPerSample: {}", limits.maxColorAttachmentBytesPerSample);
		spdlog::trace("\tmaxComputeWorkgroupStorageSize: {}", limits.maxComputeWorkgroupStorageSize);
		spdlog::trace("\tmaxComputeInvocationsPerWorkgroup: {}", limits.maxComputeInvocationsPerWorkgroup);
		spdlog::trace("\tmaxComputeWorkgroupSizeX: {}", limits.maxComputeWorkgroupSizeX);
		spdlog::trace("\tmaxComputeWorkgroupSizeY: {}", limits.maxComputeWorkgroupSizeY);
		spdlog::trace("\tmaxComputeWorkgroupSizeZ: {}", limits.maxComputeWorkgroupSizeZ);
		spdlog::trace("\tmaxComputeWorkgroupsPerDimension: {}", limits.maxComputeWorkgroupsPerDimension);
		spdlog::trace("\tmaxImmediateSize: {}", limits.maxImmediateSize);

        spdlog::trace("}");

    }
}
