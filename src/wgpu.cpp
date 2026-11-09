#include "wgpu.h"

#include <cassert>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace Chemical {
    
    std::tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue> WGPU::initialise_webgpu() {

        auto wgpu = std::make_tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue>(nullptr, nullptr, nullptr, nullptr);

        auto& [instance, adapter, device, queue] = wgpu;

        spdlog::trace("Creating WebGPU instance.");

        WGPUInstanceDescriptor desc = WGPU_INSTANCE_DESCRIPTOR_INIT;

#ifdef __EMSCRIPTEN__
        spdlog::trace("Using Emscripten WebGPU backend.");
        instance = wgpuCreateInstance(nullptr);
#else
        instance = wgpuCreateInstance(&desc);
#endif

        if(!instance)
            throw std::runtime_error("Failed to create WebGPU instance.");

        bool is_adapter_processed = false;

        spdlog::trace("Requesting WebGPU adapter.");

        WGPURequestAdapterOptions adapter_options = WGPU_REQUEST_ADAPTER_OPTIONS_INIT;

        request_adapter_async(instance, adapter, is_adapter_processed, &adapter_options);

#ifdef __EMSCRIPTEN__
        while(!is_adapter_processed) {
            emscripten_sleep(100); 
            wgpuInstanceProcessEvents(instance);
        }
#else
        wgpuInstanceProcessEvents(instance);
#endif

        if(!adapter)
            throw std::runtime_error("Failed to request WebGPU adapter.");

        spdlog::trace("Requesting WebGPU device.");

        bool is_device_processed = false;

        WGPUDeviceDescriptor device_descriptor = WGPU_DEVICE_DESCRIPTOR_INIT;
        device_descriptor.label = string_view("WGPUDevice");
        device_descriptor.defaultQueue.label= string_view("WGPUDeviceQueue");

        auto on_webgpu_device_lost = [](WGPUDevice const *, WGPUDeviceLostReason reason, WGPUStringView message, void*, void*) {

            std::string reason_str;
            switch(reason) {
                case WGPUDeviceLostReason_Unknown: 
                    reason_str = "for an unknown reason"; break;
                case WGPUDeviceLostReason_Destroyed:
                    reason_str = "because it was destroyed"; break;
                case WGPUDeviceLostReason_CallbackCancelled:
                    reason_str = "because a callback was cancelled"; break;
                case WGPUDeviceLostReason_FailedCreation:
                    reason_str = "because creation failed"; break;
                case WGPUDeviceLostReason_Force32:
                    reason_str = "because force 32"; break;
            }
            spdlog::error("WebGPU device was lost {}: {}", reason_str, message.data);
        };
        device_descriptor.deviceLostCallbackInfo.callback = on_webgpu_device_lost;
        device_descriptor.deviceLostCallbackInfo.mode = WGPUCallbackMode_AllowSpontaneous;

        request_device_async(adapter, device, is_device_processed, &device_descriptor);


#ifdef __EMSCRIPTEN__
        while(!is_device_processed) {
            emscripten_sleep(100); 
            wgpuInstanceProcessEvents(instance);
        }
#else
        wgpuInstanceProcessEvents(instance);
#endif

        if(!device)
            throw std::runtime_error("Failed to request WebGPU device.");

        queue = wgpuDeviceGetQueue(device);

        // this callback doesn't work how i expect it to anyway
        //      auto on_queue_work_done = [](WGPUQueueWorkDoneStatus, WGPUStringView message, void*, void*) {
        // spdlog::trace("Queued work finished: {}", message.data);
        //      };
        //
        //      wgpuQueueOnSubmittedWorkDone(queue, 
        //          WGPUQueueWorkDoneCallbackInfo {
        //          nullptr,
        //          WGPUCallbackMode_AllowProcessEvents,
        //          on_queue_work_done,
        //          nullptr,
        //          nullptr
        //      });
        return wgpu;
    }
    void WGPU::terminate_webgpu(std::tuple<WGPUInstance, WGPUAdapter, WGPUDevice, WGPUQueue>& wgpu) {

        auto& [instance, adapter,device, queue] = wgpu;

        spdlog::trace("Releasing WebGPU instance.");
        wgpuInstanceRelease(instance);
        spdlog::trace("Releasing WebGPU adapter.");
        wgpuAdapterRelease(adapter);
        spdlog::trace("Releasing WebGPU queue.");
        wgpuQueueRelease(queue);
        spdlog::trace("Releasing WebGPU device.");
        wgpuDeviceRelease(device);
    }

    void WGPU::request_adapter_async(WGPUInstance instance, WGPUAdapter& out_adapter, bool& out_is_adapter_processed, const WGPURequestAdapterOptions* options) {

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
                     WGPUCallbackMode_AllowProcessEvents,
                     on_adapter_request_ended,
                     &out_adapter,
                     &out_is_adapter_processed
            }
        );
    }

    void WGPU::request_device_async(WGPUAdapter& adapter, WGPUDevice& out_device, bool& out_is_device_processed, const WGPUDeviceDescriptor* descriptor) {

        auto on_device_request_ended = [](WGPURequestDeviceStatus status,
                WGPUDevice device, WGPUStringView message, 
                void* out_device, void* out_is_device_processed) {
            if (status == WGPURequestDeviceStatus_Success)
                *reinterpret_cast<WGPUDevice*>(out_device) = device;
            else
                spdlog::error("WebGPU device request failed: {}", message.data);
            *reinterpret_cast<bool*>(out_is_device_processed) = true;

        };

        wgpuAdapterRequestDevice(
            adapter, descriptor,
            WGPURequestDeviceCallbackInfo{
                nullptr,
                WGPUCallbackMode_AllowProcessEvents,
                on_device_request_ended,
                 &out_device,
                 &out_is_device_processed
            }
        );
    }

    void WGPU::print_adapter_info(WGPUAdapter& adapter) {

        auto backend_type_map = get_backend_type_map();
        auto adapter_type_map = get_adapter_type_map();
        auto feature_name_map = get_feature_name_map();

        WGPUAdapterInfo info;
        info.nextInChain = nullptr;

        wgpuAdapterGetInfo(adapter, &info);

        spdlog::info("WebGPU adapter information {");

		spdlog::info("\tvendor: {},", info.vendor.data);
		spdlog::info("\tarchitecture: {},", info.architecture.data);
		spdlog::info("\tdevice: {},", info.device.data);
		spdlog::info("\tdescription: {},", info.description.data);

        // Just let it crash honestly its so rare
		spdlog::info("\tbackendType: {},", backend_type_map.at(info.backendType));
		spdlog::info("\tadapterType: {},", adapter_type_map.at(info.adapterType));
		spdlog::info("\tvendorID: {},", info.vendorID);
		spdlog::info("\tdeviceID: {},", info.deviceID);
		spdlog::info("\tsubgroupMinSize: {},", info.subgroupMinSize);
		spdlog::info("\tsubgroupMaxSize: {},", info.subgroupMaxSize);

        spdlog::info("}");

        wgpuAdapterInfoFreeMembers(info);

        // This segfaults when initialised to nullptr?
        // Im not sure im using this API correctly or deleting it correctly
        WGPUSupportedFeatures* features = new WGPUSupportedFeatures();
        wgpuAdapterGetFeatures(adapter, features);

        spdlog::info("WebGPU adapter supported features {");

        for(size_t i = 0; i < features->featureCount; i++) {
            int feature = *(features->features + i);
            if (auto search = feature_name_map.find(feature); search != feature_name_map.end())
                spdlog::info("\t{},", search->second);
            else
                spdlog::warn("\tWebGPU adapter feature {0:x} does not appear to have a reflected name, Chemical may not support this feature.", feature);
            
        }

        spdlog::info("}");

        wgpuSupportedFeaturesFreeMembers(*features);
        delete features;

        // Does not need to be freed supposedly
        WGPULimits limits;
        limits.nextInChain = nullptr;

        wgpuAdapterGetLimits(adapter, &limits);

        spdlog::info("WebGPU adapter limits {");


		spdlog::info("\tmaxTextureDimension1D: {}", limits.maxTextureDimension1D);
		spdlog::info("\tmaxTextureDimension2D: {}", limits.maxTextureDimension2D);
		spdlog::info("\tmaxTextureDimension3D: {}", limits.maxTextureDimension3D);
		spdlog::info("\tmaxTextureArrayLayers: {}", limits.maxTextureArrayLayers);
		spdlog::info("\tmaxBindGroups: {}", limits.maxBindGroups);
		spdlog::info("\tmaxBindGroupsPlusVertexBuffers: {}", limits.maxBindGroupsPlusVertexBuffers);
		spdlog::info("\tmaxBindingsPerBindGroup: {}", limits.maxBindingsPerBindGroup);
		spdlog::info("\tmaxDynamicUniformBuffersPerPipelineLayout: {}", limits.maxDynamicUniformBuffersPerPipelineLayout);
		spdlog::info("\tmaxDynamicStorageBuffersPerPipelineLayout: {}", limits.maxDynamicStorageBuffersPerPipelineLayout);
		spdlog::info("\tmaxSampledTexturesPerShaderStage: {}", limits.maxSampledTexturesPerShaderStage);
		spdlog::info("\tmaxSamplersPerShaderStage: {}", limits.maxSamplersPerShaderStage);
		spdlog::info("\tmaxStorageBuffersPerShaderStage: {}", limits.maxStorageBuffersPerShaderStage);
		spdlog::info("\tmaxStorageTexturesPerShaderStage: {}", limits.maxStorageTexturesPerShaderStage);
		spdlog::info("\tmaxUniformBuffersPerShaderStage: {}", limits.maxUniformBuffersPerShaderStage);
		spdlog::info("\tmaxUniformBufferBindingSize: {}", limits.maxUniformBufferBindingSize);
		spdlog::info("\tmaxStorageBufferBindingSize: {}", limits.maxStorageBufferBindingSize);
		spdlog::info("\tminUniformBufferOffsetAlignment: {}", limits.minUniformBufferOffsetAlignment);
		spdlog::info("\tminStorageBufferOffsetAlignment: {}", limits.minStorageBufferOffsetAlignment);
		spdlog::info("\tmaxVertexBuffers: {}", limits.maxVertexBuffers);
		spdlog::info("\tmaxBufferSize: {}", limits.maxBufferSize);
		spdlog::info("\tmaxVertexAttributes: {}", limits.maxVertexAttributes);
		spdlog::info("\tmaxVertexBufferArrayStride: {}", limits.maxVertexBufferArrayStride);
		spdlog::info("\tmaxInterStageShaderVariables: {}", limits.maxInterStageShaderVariables);
		spdlog::info("\tmaxColorAttachments: {}", limits.maxColorAttachments);
		spdlog::info("\tmaxColorAttachmentBytesPerSample: {}", limits.maxColorAttachmentBytesPerSample);
		spdlog::info("\tmaxComputeWorkgroupStorageSize: {}", limits.maxComputeWorkgroupStorageSize);
		spdlog::info("\tmaxComputeInvocationsPerWorkgroup: {}", limits.maxComputeInvocationsPerWorkgroup);
		spdlog::info("\tmaxComputeWorkgroupSizeX: {}", limits.maxComputeWorkgroupSizeX);
		spdlog::info("\tmaxComputeWorkgroupSizeY: {}", limits.maxComputeWorkgroupSizeY);
		spdlog::info("\tmaxComputeWorkgroupSizeZ: {}", limits.maxComputeWorkgroupSizeZ);
		spdlog::info("\tmaxComputeWorkgroupsPerDimension: {}", limits.maxComputeWorkgroupsPerDimension);
		spdlog::info("\tmaxImmediateSize: {}", limits.maxImmediateSize);

        spdlog::info("}");

    }

    WGPUStringView WGPU::string_view(const std::string_view& string) {
        return WGPUStringView {
            string.data(),
            string.size()
        };
    }

    std::unordered_map<int, const char*>& WGPU::get_feature_name_map() {

        static std::unique_ptr<std::unordered_map<int, const char*>> feature_names;
        if(feature_names != nullptr)
            return *feature_names;
            
        feature_names = std::make_unique<std::unordered_map<int, const char*>>(
               std::unordered_map<int, const char*>{
			{0x00000001, "WGPUFeatureName_CoreFeaturesAndLimits"},
			{0x00000002, "WGPUFeatureName_DepthClipControl"},
			{0x00000003, "WGPUFeatureName_Depth32FloatStencil8"},
			{0x00000004, "WGPUFeatureName_TextureCompressionBC"},
			{0x00000005, "WGPUFeatureName_TextureCompressionBCSliced3D"},
			{0x00000006, "WGPUFeatureName_TextureCompressionETC2"},
			{0x00000007, "WGPUFeatureName_TextureCompressionASTC"},
			{0x00000008, "WGPUFeatureName_TextureCompressionASTCSliced3D"},
			{0x00000009, "WGPUFeatureName_TimestampQuery"},
			{0x0000000A, "WGPUFeatureName_IndirectFirstInstance"},
			{0x0000000B, "WGPUFeatureName_ShaderF16"},
			{0x0000000C, "WGPUFeatureName_RG11B10UfloatRenderable"},
			{0x0000000D, "WGPUFeatureName_BGRA8UnormStorage"},
			{0x0000000E, "WGPUFeatureName_Float32Filterable"},
			{0x0000000F, "WGPUFeatureName_Float32Blendable"},
			{0x00000010, "WGPUFeatureName_ClipDistances"},
			{0x00000011, "WGPUFeatureName_DualSourceBlending"},
			{0x00000012, "WGPUFeatureName_Subgroups"},
			{0x00000013, "WGPUFeatureName_TextureFormatsTier1"},
			{0x00000014, "WGPUFeatureName_TextureFormatsTier2"},
			{0x00000015, "WGPUFeatureName_PrimitiveIndex"},
			{0x00000016, "WGPUFeatureName_TextureComponentSwizzle"},
			{0x00000017, "WGPUFeatureName_SubgroupSizeControl"},
			{0x00000018, "WGPUFeatureName_TextureCompressionUnaligned"},
			{0x00000019, "WGPUFeatureName_AtomicVec2uMinMax"},
			{0x00050000, "WGPUFeatureName_DawnInternalUsages"},
			{0x00050001, "WGPUFeatureName_DawnMultiPlanarFormats"},
			{0x00050002, "WGPUFeatureName_DawnNative"},
			{0x00050003, "WGPUFeatureName_ChromiumExperimentalTimestampQueryInsidePasses"},
			{0x00050004, "WGPUFeatureName_ImplicitDeviceSynchronization"},
			{0x00050006, "WGPUFeatureName_TransientAttachments"},
			{0x00050007, "WGPUFeatureName_MSAARenderToSingleSampled"},
			{0x00050008, "WGPUFeatureName_D3D11MultithreadProtected"},
			{0x00050009, "WGPUFeatureName_ANGLETextureSharing"},
			{0x0005000A, "WGPUFeatureName_PixelLocalStorageCoherent"},
			{0x0005000B, "WGPUFeatureName_PixelLocalStorageNonCoherent"},
			{0x0005000C, "WGPUFeatureName_Unorm16TextureFormats"},
			{0x0005000D, "WGPUFeatureName_MultiPlanarFormatExtendedUsages"},
			{0x0005000E, "WGPUFeatureName_MultiPlanarFormatP010"},
			{0x0005000F, "WGPUFeatureName_HostMappedPointer"},
			{0x00050010, "WGPUFeatureName_MultiPlanarRenderTargets"},
			{0x00050011, "WGPUFeatureName_MultiPlanarFormatNv12a"},
			{0x00050012, "WGPUFeatureName_FramebufferFetch"},
			{0x00050013, "WGPUFeatureName_BufferMapExtendedUsages"},
			{0x00050014, "WGPUFeatureName_AdapterPropertiesMemoryHeaps"},
			{0x00050015, "WGPUFeatureName_AdapterPropertiesD3D"},
			{0x00050016, "WGPUFeatureName_AdapterPropertiesVk"},
			{0x00050017, "WGPUFeatureName_DawnFormatCapabilities"},
			{0x00050018, "WGPUFeatureName_DawnDrmFormatCapabilities"},
			{0x00050019, "WGPUFeatureName_MultiPlanarFormatNv16"},
			{0x0005001A, "WGPUFeatureName_MultiPlanarFormatNv24"},
			{0x0005001B, "WGPUFeatureName_MultiPlanarFormatP210"},
			{0x0005001C, "WGPUFeatureName_MultiPlanarFormatP410"},
			{0x0005001D, "WGPUFeatureName_SharedTextureMemoryVkDedicatedAllocation"},
			{0x0005001E, "WGPUFeatureName_SharedTextureMemoryAHardwareBuffer"},
			{0x0005001F, "WGPUFeatureName_SharedTextureMemoryDmaBuf"},
			{0x00050020, "WGPUFeatureName_SharedTextureMemoryOpaqueFD"},
			{0x00050021, "WGPUFeatureName_SharedTextureMemoryZirconHandle"},
			{0x00050022, "WGPUFeatureName_SharedTextureMemoryDXGISharedHandle"},
			{0x00050023, "WGPUFeatureName_SharedTextureMemoryD3D11Texture2D"},
			{0x00050024, "WGPUFeatureName_SharedTextureMemoryIOSurface"},
			{0x00050025, "WGPUFeatureName_SharedTextureMemoryEGLImage"},
			{0x00050026, "WGPUFeatureName_SharedFenceVkSemaphoreOpaqueFD"},
			{0x00050027, "WGPUFeatureName_SharedFenceSyncFD"},
			{0x00050028, "WGPUFeatureName_SharedFenceVkSemaphoreZirconHandle"},
			{0x00050029, "WGPUFeatureName_SharedFenceDXGISharedHandle"},
			{0x0005002A, "WGPUFeatureName_SharedFenceMTLSharedEvent"},
			{0x0005002B, "WGPUFeatureName_SharedBufferMemoryD3D12Resource"},
			{0x0005002C, "WGPUFeatureName_StaticSamplers"},
			{0x0005002D, "WGPUFeatureName_YCbCrVulkanSamplers"},
			{0x0005002E, "WGPUFeatureName_ShaderModuleCompilationOptions"},
			{0x0005002F, "WGPUFeatureName_DawnLoadResolveTexture"},
			{0x00050030, "WGPUFeatureName_DawnPartialLoadResolveTexture"},
			{0x00050031, "WGPUFeatureName_MultiDrawIndirect"},
			{0x00050032, "WGPUFeatureName_DawnTexelCopyBufferRowAlignment"},
			{0x00050033, "WGPUFeatureName_FlexibleTextureViews"},
			{0x00050034, "WGPUFeatureName_ChromiumExperimentalSubgroupMatrix"},
			{0x00050035, "WGPUFeatureName_SharedFenceEGLSync"},
			{0x00050036, "WGPUFeatureName_DawnDeviceAllocatorControl"},
			{0x00050037, "WGPUFeatureName_AdapterPropertiesWGPU"},
			{0x00050038, "WGPUFeatureName_SharedBufferMemoryFromWindowsHandle"},
			{0x00050039, "WGPUFeatureName_SharedTextureMemoryD3D12Resource"},
			{0x0005003A, "WGPUFeatureName_ChromiumExperimentalSamplingResourceTable"},
			{0x0005003B, "WGPUFeatureName_Unorm16FormatsForExternalTexture"},
			{0x0005003C, "WGPUFeatureName_OpaqueYCbCrAndroidForExternalTexture"},
			{0x0005003D, "WGPUFeatureName_Unorm16Filterable"},
			{0x0005003E, "WGPUFeatureName_RenderPassRenderArea"},
			{0x0005003F, "WGPUFeatureName_AdapterPropertiesDrm"},
			{0x00050040, "WGPUFeatureName_DawnAllowUndefinedLoadStoreOp"},
			{0x00050041, "WGPUFeatureName_BufferMapWriteExtendedUsages"},
			{0x00050042, "WGPUFeatureName_SharedBufferMemoryHostPointer"},
            {0x7FFFFFFF, "WGPUFeatureName_Force32"}
        });
        
        return *feature_names;
    }

    std::unordered_map<int, const char*>& WGPU::get_backend_type_map() {
        static std::unique_ptr<std::unordered_map<int, const char*>> backend_types;
        if(backend_types != nullptr)
            return *backend_types;

        backend_types = std::make_unique<std::unordered_map<int, const char*>>(std::unordered_map<int, const char*>{
			{0x00000000, "WGPUBackendType_Undefined"},
			{0x00000001, "WGPUBackendType_Null"},
			{0x00000002, "WGPUBackendType_WebGPU"},
			{0x00000003, "WGPUBackendType_D3D11"},
			{0x00000004, "WGPUBackendType_D3D12"},
			{0x00000005, "WGPUBackendType_Metal"},
			{0x00000006, "WGPUBackendType_Vulkan"},
			{0x00000007, "WGPUBackendType_OpenGL"},
			{0x00000008, "WGPUBackendType_OpenGLES"},
			{0x7FFFFFFF, "WGPUBackendType_Force32"}
        });
        return *backend_types;
    }

    std::unordered_map<int, const char*>& WGPU::get_adapter_type_map() {
        static std::unique_ptr<std::unordered_map<int, const char*>> adapter_types;
        if(adapter_types != nullptr)
            return *adapter_types;

        adapter_types = std::make_unique<std::unordered_map<int, const char*>>(std::unordered_map<int, const char*>{
			{0x00000001, "WGPUAdapterType_DiscreteGPU"},
			{0x00000002, "WGPUAdapterType_IntegratedGPU"},
			{0x00000003, "WGPUAdapterType_CPU"},
			{0x00000004, "WGPUAdapterType_Unknown"},
			{0x7FFFFFFF, "WGPUAdapterType_Force32"}
        });

        return *adapter_types;
    }
}
