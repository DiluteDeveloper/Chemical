#include "wgpu.h"

#include <cassert>
#include <spdlog/spdlog.h>
#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace Chemical {
    
    std::tuple<WGPUInstance, WGPUAdapter, 
        WGPUDevice, WGPUQueue> WGPU::initialise_webgpu() {

        auto wgpu = std::make_tuple<WGPUInstance, WGPUAdapter, 
        WGPUDevice, WGPUQueue>(nullptr, nullptr, nullptr, nullptr);
        auto& [instance, adapter, device, queue] = wgpu;

        spdlog::trace("Creating WebGPU instance.");

        WGPUInstanceDescriptor desc = WGPU_INSTANCE_DESCRIPTOR_INIT;

#ifdef __EMSCRIPTEN__
        spdlog::trace("Using Emscripten WebGPU backend.");
        WGPUInstance wgpu_instance = wgpuCreateInstance(nullptr);
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
        while(!requests.is_adapter_processed) {
            emscripten_sleep(100); 
            wgpuInstanceProcessEvents(wgpu_instance);
        }
#else
        wgpuInstanceProcessEvents(instance);
#endif

        if(!adapter)
            throw std::runtime_error("Failed to request WebGPU adapter.");

        // print_adapter_info(.adapter);

        // spdlog::trace("Releasing WebGPU instance.");

        // Drop down reference count and release if 0 (Referenced by adapter so released with adapter now)
        // wgpuInstanceRelease(wgpu_instance);

        spdlog::trace("Requesting WebGPU device.");

        bool is_device_processed = false;

        WGPUDeviceDescriptor device_descriptor = WGPU_DEVICE_DESCRIPTOR_INIT;
        device_descriptor.label = create_webgpu_string_view("WGPUDevice");
        device_descriptor.defaultQueue.label= create_webgpu_string_view("WGPUDeviceQueue");

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
        while(!requests.is_adapter_processed) {
            emscripten_sleep(100); 
            wgpuInstanceProcessEvents(wgpu_instance);
        }
#else
        wgpuInstanceProcessEvents(instance);
#endif

        if(!device)
            throw std::runtime_error("Failed to request WebGPU device.");

        // spdlog::trace("Releasing WebGPU adapter.");
        // wgpuAdapterRelease(requests.adapter);
        
        queue = wgpuDeviceGetQueue(device);

        return wgpu;
    }

    //
    //     spdlog::trace("Releasing WebGPU device.");
    //     wgpuDeviceRelease(device);
    // }

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

        spdlog::trace("WebGPU adapter information {");

		spdlog::trace("\tvendor: {},", info.vendor.data);
		spdlog::trace("\tarchitecture: {},", info.architecture.data);
		spdlog::trace("\tdevice: {},", info.device.data);
		spdlog::trace("\tdescription: {},", info.description.data);

        // Just let it crash honestly its so rare
		spdlog::trace("\tbackendType: {},", backend_type_map.at(info.backendType));
		spdlog::trace("\tadapterType: {},", adapter_type_map.at(info.adapterType));
		spdlog::trace("\tvendorID: {},", info.vendorID);
		spdlog::trace("\tdeviceID: {},", info.deviceID);
		spdlog::trace("\tsubgroupMinSize: {},", info.subgroupMinSize);
		spdlog::trace("\tsubgroupMaxSize: {},", info.subgroupMaxSize);

        spdlog::trace("}");

        wgpuAdapterInfoFreeMembers(info);

        // This segfaults when initialised to nullptr?
        // Im not sure im using this API correctly or deleting it correctly
        WGPUSupportedFeatures* features = new WGPUSupportedFeatures();
        wgpuAdapterGetFeatures(adapter, features);

        spdlog::trace("WebGPU adapter supported features {");

        for(size_t i = 0; i < features->featureCount; i++) {
            int feature = *(features->features + i);
            if (auto search = feature_name_map.find(feature); search != feature_name_map.end())
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
    WGPUStringView WGPU::create_webgpu_string_view(const std::string_view& string) {
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
