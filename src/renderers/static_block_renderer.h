#pragma once

#include "renderer_data.h"
namespace Chemical {

    class StaticBlockRenderer {
    public:
        void Initialise(const RendererInitData& init);
        void Render(wgpu::RenderPassEncoder render_pass);

    private:
        
        wgpu::RenderPipeline pipeline;
        wgpu::BindGroup group;
        uint32_t vertex_count;
    };
}
