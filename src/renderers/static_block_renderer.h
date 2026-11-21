#pragma once

#include "block.h"
#include "renderer.h"

namespace Chemical {

    // Closely coupled with the root renderer
    class StaticBlockRenderer {
    public:
        friend class Renderer;

        StaticBlockRenderer(const Renderer&);

    private:
        void Render(wgpu::RenderPassEncoder);
        void ReloadBlockData(entt::registry&, const entt::entity);

        wgpu::RenderPipeline pipeline;
        wgpu::BindGroup group;
        wgpu::BindGroupLayout layout;
        wgpu::Buffer block_data_buffer;
        uint32_t vertex_count;

        const Renderer* renderer;

		[[nodiscard]] static wgpu::RenderPipeline CreateRenderPipeline(const Renderer&, const wgpu::BindGroupLayout);
		[[nodiscard]] static std::tuple<uint32_t, wgpu::BindGroup, wgpu::Buffer> CreateBufferAndBindGroup(const Renderer&, std::span<BlockData>, wgpu::BindGroupLayout);
		[[nodiscard]] static wgpu::BindGroupLayout CreateBindGroupLayout(const Renderer&);
		[[nodiscard]] static std::span<BlockData> GetRegistryBlockData(entt::registry& registry);
        [[nodiscard]] static std::tuple<wgpu::Texture, wgpu::TextureDescriptor> SetupTexture(const Renderer& renderer);
    };
}

