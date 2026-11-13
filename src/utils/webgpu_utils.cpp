#include "webgpu_utils.h"

#include <spdlog/spdlog.h>
#include <webgpu/webgpu_cpp.h>

#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
#endif

namespace Chemical {

   namespace Utils {

       void WebGPU::PrintAdapterInfo(wgpu::Adapter& adapter) {

            auto backend_type_map = CreateBackendTypeMap();
            auto adapter_type_map = CreateAdapterTypeMap();
            auto feature_name_map = CreateFeatureNameMap();

            wgpu::AdapterInfo info;
            info.nextInChain = nullptr;

            adapter.GetInfo(&info);

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

            wgpu::SupportedFeatures features;
            adapter.GetFeatures(&features);

            spdlog::info("WebGPU adapter supported features {");

            for(size_t i = 0; i < features.featureCount; i++) {
                int feature = (int)*(features.features + i);
                if (auto search = feature_name_map.find(feature); search != feature_name_map.end())
                    spdlog::info("\t{},", search->second);
                else
                    spdlog::warn("\tWebGPU adapter feature {0:x} does not appear to have a reflected name, Chemical may not support this feature.", feature);

            }

            spdlog::info("}");

            wgpu::Limits limits;

            adapter.GetLimits(&limits);

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

        std::tuple<wgpu::SurfaceTexture, wgpu::TextureView> WebGPU::GetNextSurfaceViewData(const wgpu::Surface& surface) {
            wgpu::SurfaceTexture surface_texture;
            surface.GetCurrentTexture(&surface_texture);
            if(surface_texture.status != wgpu::SurfaceGetCurrentTextureStatus::SuccessOptimal) {
                spdlog::warn("Surface texture status is not success optimal");
                return std::make_tuple(surface_texture, nullptr);
            }

            wgpu::TextureViewDescriptor view_descriptor;
            view_descriptor.format = surface_texture.texture.GetFormat();
            view_descriptor.dimension = wgpu::TextureViewDimension::e2D;
            view_descriptor.mipLevelCount = 1;
            view_descriptor.arrayLayerCount = 1;
            view_descriptor.aspect = wgpu::TextureAspect::All;
            wgpu::TextureView target_view = surface_texture.texture.CreateView(&view_descriptor);
            
            return std::make_tuple(surface_texture, target_view);
        }

        std::unordered_map<int, std::string> WebGPU::CreateFeatureNameMap() {

            return std::unordered_map<int, std::string>{
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
                {0x7FFFFFFF, "WGPUFeatureName_Force32"},
            };
        }

        std::unordered_map<wgpu::BackendType, std::string> WebGPU::CreateBackendTypeMap() {

            return std::unordered_map<wgpu::BackendType, std::string>{
                {wgpu::BackendType::Undefined, "wgpu::BackendType::Undefined"},
                {wgpu::BackendType::Null, "wgpu::BackendType::Null"},
                {wgpu::BackendType::WebGPU, "wgpu::BackendType::WebGPU"},
                {wgpu::BackendType::D3D11, "wgpu::BackendType::D3D11"},
                {wgpu::BackendType::D3D12, "wgpu::BackendType::D3D12"},
                {wgpu::BackendType::Metal, "wgpu::BackendType::Metal"},
                {wgpu::BackendType::Vulkan, "wgpu::BackendType::Vulkan"},
                {wgpu::BackendType::OpenGL, "wgpu::BackendType::OpenGL"},
                {wgpu::BackendType::OpenGLES, "wgpu::BackendType::OpenGLES"}
            };
        }

        std::unordered_map<wgpu::AdapterType, std::string> WebGPU::CreateAdapterTypeMap() {
            return std::unordered_map<wgpu::AdapterType, std::string>{
                {wgpu::AdapterType::DiscreteGPU, "wgpu::AdapterType::DiscreteGPU"},
                {wgpu::AdapterType::IntegratedGPU, "wgpu::AdapterType::IntegratedGPU"},
                {wgpu::AdapterType::CPU, "wgpu::AdapterType::CPU"},
                {wgpu::AdapterType::Unknown, "wgpu::AdapterType::Unknown"}
            };
        }
        void WebGPU::AwaitAsync(const wgpu::Instance& instance, const wgpu::Future& future) {

            while(true) {
#ifdef __EMSCRIPTEN__
                emscripten_sleep(10); 
#endif
                if(wgpu::WaitStatus s = instance.WaitAny(future, 0); s == wgpu::WaitStatus::Success)
                    return;
            }


        }

        wgpu::Limits WebGPU::CreateRequiredLimits() {
            // I will probably not update this
            wgpu::Limits limits {
                .maxVertexBuffers = 1,
                .maxBufferSize = 5000 * sizeof(float),
                .maxVertexAttributes = 2,
                .maxVertexBufferArrayStride = 6 * sizeof(float),
                .maxInterStageShaderVariables = 3
            };

            return limits;
        }
    } 
}
