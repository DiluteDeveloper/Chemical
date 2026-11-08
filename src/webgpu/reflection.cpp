#include "chemical_webgpu.h"



namespace Chemical {

    std::unique_ptr<std::unordered_map<int, const char*>> WebGPU::feature_names;
    std::unique_ptr<std::unordered_map<int, const char*>> WebGPU::backend_types;
    std::unique_ptr<std::unordered_map<int, const char*>> WebGPU::adapter_types;

    void WebGPU::set_feature_names() {
        if(feature_names != nullptr)
            return;

        feature_names = std::make_unique<std::unordered_map<int, const char*>>(std::unordered_map<int, const char*>{
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

    }
    void WebGPU::set_backend_types() {
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
    }
    void WebGPU::set_adapter_types() {
        adapter_types = std::make_unique<std::unordered_map<int, const char*>>(std::unordered_map<int, const char*>{
			{0x00000001, "WGPUAdapterType_DiscreteGPU"},
			{0x00000002, "WGPUAdapterType_IntegratedGPU"},
			{0x00000003, "WGPUAdapterType_CPU"},
			{0x00000004, "WGPUAdapterType_Unknown"},
			{0x7FFFFFFF, "WGPUAdapterType_Force32"}
        });
    }
}
