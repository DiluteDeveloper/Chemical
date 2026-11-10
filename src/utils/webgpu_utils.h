#pragma once

#include <webgpu/webgpu_cpp.h>

namespace Chemical {

    namespace Utils {
        class WebGPU {
        public:

            static void PrintAdapterInfo(wgpu::Adapter& adapter);
            static std::tuple<wgpu::SurfaceTexture, wgpu::TextureView> GetNextSurfaceViewData(const wgpu::Surface& surface);

            // static wgpu::StringView CreateStringView(const std::string_view& string);
        private:

            static std::unordered_map<int, 
                std::string> CreateFeatureNameMap();
            static std::unordered_map<wgpu::BackendType, 
                std::string> CreateBackendTypeMap();
            static std::unordered_map<wgpu::AdapterType, 
                std::string> CreateAdapterTypeMap();

        };
    }

}

