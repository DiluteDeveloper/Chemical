#pragma once

#include "webgpu/webgpu_cpp.h"
namespace Chemical {

    struct RendererInitData {
        wgpu::Instance instance;
        wgpu::Adapter adapter;
        wgpu::Device device;
        wgpu::Queue queue;
        wgpu::TextureFormat surface_format;
        wgpu::Surface surface;
        wgpu::BindGroupLayout view_projection_layout;
    };
}
