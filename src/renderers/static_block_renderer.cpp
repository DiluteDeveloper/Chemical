#include "renderer.h"
#include "block.h"
#include "utils/image.h"
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>
#include <fstream>

namespace {

    using namespace Chemical;
    wgpu::RenderPipeline CreateRenderPipeline(const RendererInitData& init, const wgpu::BindGroupLayout layout);
    std::tuple<uint32_t, wgpu::BindGroup, wgpu::Buffer> CreateBufferAndBindGroup(const RendererInitData& init, std::span<BlockData> block_data, wgpu::BindGroupLayout layout);
    wgpu::BindGroupLayout CreateBindGroupLayout(const RendererInitData& init);
    std::span<BlockData> GetRegistryBlockData();

    wgpu::RenderPipeline pipeline;
    wgpu::BindGroup group;
    wgpu::BindGroupLayout layout;
    wgpu::Buffer block_data_buffer;
    uint32_t vertex_count;

    bool renderer_initialised = false;
    entt::registry* registry;
}
namespace Chemical::StaticBlockRenderer {

    void Initialise(const RendererInitData& init, entt::registry& _registry) {

        registry = &_registry;
        layout = CreateBindGroupLayout(init);
        std::span<BlockData> bd = GetRegistryBlockData();
        auto [_vertex_count, _group, _block_data_buffer] = CreateBufferAndBindGroup(Renderer::init, bd, layout);
        
        pipeline = CreateRenderPipeline(init, layout);
        vertex_count = _vertex_count;
        group = _group;
        block_data_buffer = _block_data_buffer;

        renderer_initialised = true;

    }
    void Render(wgpu::RenderPassEncoder render_pass) {
        render_pass.SetPipeline(pipeline);
        render_pass.SetBindGroup(1, group, 0, nullptr);
        render_pass.Draw(vertex_count, 1, 0, 0);
    }
}

void BlockData::on_construct(entt::registry&, const entt::entity) {
    if(!renderer_initialised)
        return;
    std::span<BlockData> bd = GetRegistryBlockData();
    auto [_vertex_count, _group, _block_data_buffer] = CreateBufferAndBindGroup(Renderer::init, bd, layout);
    vertex_count = _vertex_count;
    group = _group;
    block_data_buffer = _block_data_buffer;
}
void BlockData::on_update(entt::registry &, const entt::entity) {
    if(!renderer_initialised)
        return;
    std::span<BlockData> bd = GetRegistryBlockData();
    auto [_vertex_count, _group, _block_data_buffer] = CreateBufferAndBindGroup(Renderer::init, bd, layout);
    vertex_count = _vertex_count;
    group = _group;
    block_data_buffer = _block_data_buffer;
}

namespace {

    wgpu::RenderPipeline CreateRenderPipeline(const RendererInitData& init, const wgpu::BindGroupLayout layout) {
        std::ifstream shader_file("res/shaders/main.wgsl");
        if (!shader_file.is_open())
            throw std::runtime_error("Failed to read file: res/shaders/main.wgsl");

        std::string shader_source((std::istreambuf_iterator<char>(shader_file)),
            std::istreambuf_iterator<char>());
        
        wgpu::ShaderSourceWGSL shader_code({ .nextInChain = nullptr, .code = shader_source.data() });
        
        wgpu::ShaderModuleDescriptor shader_desc{
            .nextInChain = &shader_code,
        };
        
        wgpu::ShaderModule shader_module = init.device.CreateShaderModule(&shader_desc);
        
        wgpu::BlendState blend{
            .color = {
                .operation = wgpu::BlendOperation::Add,
                .srcFactor = wgpu::BlendFactor::SrcAlpha,
                .dstFactor = wgpu::BlendFactor::OneMinusSrcAlpha,
            },
            .alpha = {
                .operation = wgpu::BlendOperation::Add,
                .srcFactor = wgpu::BlendFactor::Zero,
                .dstFactor = wgpu::BlendFactor::One,
            },
        };
        
        wgpu::ColorTargetState color_target{
            .format = init.surface_format,
            .blend = &blend,
            .writeMask = wgpu::ColorWriteMask::All,
        };
        
        wgpu::FragmentState frag{
            .module = shader_module,
            .entryPoint = "fs_main",
            .constantCount = 0,
            .constants = nullptr,
            .targetCount = 1,
            .targets = &color_target,
        };

        std::array<wgpu::BindGroupLayout, 2> bind_group_layouts = {
                init.view_projection_layout, layout
        };
        wgpu::PipelineLayoutDescriptor pld {
            .bindGroupLayoutCount = 2,
            .bindGroupLayouts = &bind_group_layouts[0],
        };
        wgpu::DepthStencilState ds = {
            .format = wgpu::TextureFormat::Depth32Float,
            .depthWriteEnabled = wgpu::OptionalBool::True,
            .depthCompare = wgpu::CompareFunction::Less
        };
        wgpu::RenderPipelineDescriptor rp_desc{
            .layout = init.device.CreatePipelineLayout(&pld),
            .vertex = {
                .module = shader_module,
                .entryPoint = "vs_main",
                .constantCount = 0,
                .constants = nullptr,
                .bufferCount = 0,
                .buffers = nullptr,
            },
            .primitive = {
                .topology = wgpu::PrimitiveTopology::TriangleList,
                .stripIndexFormat = wgpu::IndexFormat::Undefined,
                .frontFace = wgpu::FrontFace::CCW,
                .cullMode = wgpu::CullMode::Back,
            },
            .depthStencil = &ds,
            .multisample = {
                .count = 1,
                .mask = ~0u,
                .alphaToCoverageEnabled = false,
            },
            .fragment = &frag

        };
        
        return init.device.CreateRenderPipeline(&rp_desc);    
    }

    std::vector<glm::ivec4> BlockFaceToShaderData(const std::span<BlockData> data) {
        static constexpr std::array<std::array<uint8_t, 2>, 6> face_orientation_to_position_dimension_idx = {{
            {0, 2}, // FaceOrientation::Top
            {2, 0}, // FaceOrientation::Bottom
            {2, 1}, // FaceOrientation::Right
            {1, 2}, // FaceOrientation::Left
            {0, 1}, // FaceOrientation::Back
            {1, 0} // FaceOrientation::Front
        }};

        std::vector<glm::ivec4> r;
        for(auto d : data) {
            auto& arr = face_orientation_to_position_dimension_idx[(uint8_t)d.orientation];

            auto swap = [](int32_t& a, int32_t& b) {
                int32_t v = a;
                a = b;
                b = v;
            };
            if(d.end_position.x < d.start_position.x)
                swap(d.end_position.x, d.start_position.x);
            if(d.end_position.y < d.start_position.y)
                swap(d.end_position.y, d.start_position.y);
            if(d.end_position.z < d.start_position.z)
                swap(d.end_position.z, d.start_position.z);

            r.emplace_back(d.start_position.x, d.start_position.y, d.start_position.z, d.type);

            r.emplace_back((uint32_t)d.orientation, 
                    std::abs(d.end_position[arr[0]] - d.start_position[arr[0]]), 
                    std::abs(d.end_position[arr[1]] - d.start_position[arr[1]]), 0);
        }

        return r;

    }

    std::tuple<wgpu::Texture, wgpu::TextureDescriptor> SetupTexture(const RendererInitData& init) {

        auto image = Image::LoadImage("res/textures/blocks/atlas.png", 4);
        if(!image)
            throw std::runtime_error(std::format("Failed to load image {}", "res/textures/blocks/bedrock.png"));

        constexpr uint32_t ATLAS_UNIT_PIXEL_SIZE = 32;
        if(image->x % ATLAS_UNIT_PIXEL_SIZE != 0 || image->y % ATLAS_UNIT_PIXEL_SIZE != 0)
            throw std::runtime_error("Failed to load texture! Texture size is not a multiple of 32!");

        uint32_t atlas_units_x = image->x / ATLAS_UNIT_PIXEL_SIZE;
        uint32_t atlas_units_y = image->y / ATLAS_UNIT_PIXEL_SIZE;
        uint32_t unit_byte_offset = image->num_channels*ATLAS_UNIT_PIXEL_SIZE;
        uint32_t image_byte_size = image->x*image->y*image->num_channels;

        wgpu::TextureDescriptor desc {
            .usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst,
            .dimension = wgpu::TextureDimension::e2D,
            .size = { ATLAS_UNIT_PIXEL_SIZE, ATLAS_UNIT_PIXEL_SIZE, atlas_units_x * atlas_units_y},
#ifdef WSL
            .format = wgpu::TextureFormat::RGBA8UnormSrgb,
#else
            .format = wgpu::TextureFormat::RGBA8Unorm,
#endif
            .mipLevelCount = 1,
            .sampleCount = 1,
        };

        wgpu::Texture texture = init.device.CreateTexture(&desc);

        wgpu::TexelCopyTextureInfo destination {
            .texture = texture,
            .mipLevel = 0,
            .origin = {0,0,0},
            .aspect = wgpu::TextureAspect::All,
        };
        wgpu::TexelCopyBufferLayout layout {
            .bytesPerRow = image->num_channels*image->x,
            .rowsPerImage = image->y 
        };
        wgpu::Extent3D extent = {ATLAS_UNIT_PIXEL_SIZE, ATLAS_UNIT_PIXEL_SIZE, 1};


        for(uint32_t iy = 0; iy < atlas_units_y; iy++) {
            for(uint32_t ix = 0; ix < atlas_units_x; ix++) {

                layout.offset = ix*unit_byte_offset + (iy*atlas_units_x*unit_byte_offset*ATLAS_UNIT_PIXEL_SIZE);
                destination.origin.z = ix + (iy*atlas_units_x);
                init.queue.WriteTexture(&destination, image->data, image_byte_size, &layout, &extent);
            }
        }

        return {texture, desc};
    }

    wgpu::BindGroupLayout CreateBindGroupLayout(const RendererInitData& init) {
        wgpu::BindGroupLayoutEntry layout_entries[] = {
            {
                .binding = 0,
                .visibility = wgpu::ShaderStage::Vertex,
                .buffer = { nullptr, wgpu::BufferBindingType::ReadOnlyStorage, false, 0}
            },
            {
                .binding = 1,
                .visibility = wgpu::ShaderStage::Fragment,
                .texture {
                    .sampleType = wgpu::TextureSampleType::Float,
                    .viewDimension = wgpu::TextureViewDimension::e2DArray
                }
            },
            {
                .binding = 2,
                .visibility = wgpu::ShaderStage::Fragment,
                .sampler { .type = wgpu::SamplerBindingType::Filtering }
            }
        };
        wgpu::BindGroupLayoutDescriptor layout_descriptor {
            .entryCount = 3,
            .entries = &layout_entries[0]
        };
        return init.device.CreateBindGroupLayout(&layout_descriptor);
    }
    std::tuple<uint32_t, wgpu::BindGroup, wgpu::Buffer> CreateBufferAndBindGroup(const RendererInitData& init, std::span<BlockData> block_data, wgpu::BindGroupLayout layout) {

        auto data = BlockFaceToShaderData(block_data);

        wgpu::BufferDescriptor bd;
        bd.size = data.size() * sizeof(glm::ivec4);
        bd.usage = wgpu::BufferUsage::Storage;
        bd.mappedAtCreation = true;

        wgpu::Buffer buffer = init.device.CreateBuffer(&bd);
        uint32_t vertex_count = (uint32_t)data.size() * 3;

        buffer.WriteMappedRange(0, data.data(), bd.size);

        buffer.Unmap();

        auto[texture, texture_desc] = SetupTexture(init);
        wgpu::TextureViewDescriptor texture_view_desc = {
            .format = texture_desc.format,
            .dimension = wgpu::TextureViewDimension::e2DArray,
            .mipLevelCount = 1,
            .arrayLayerCount = texture_desc.size.depthOrArrayLayers,
            .aspect = wgpu::TextureAspect::All,
        };
        wgpu::TextureView tv = texture.CreateView(&texture_view_desc);

        wgpu::SamplerDescriptor sd = {
            .addressModeU = wgpu::AddressMode::Repeat,
            .addressModeV = wgpu::AddressMode::Repeat,
            .addressModeW = wgpu::AddressMode::Repeat,
            .magFilter = wgpu::FilterMode::Nearest,
            .minFilter = wgpu::FilterMode::Nearest,
            .mipmapFilter = wgpu::MipmapFilterMode::Linear,
            .lodMaxClamp = 1.0f,
        };

        wgpu::Sampler s = init.device.CreateSampler(&sd);

        wgpu::BindGroupEntry group_entries[] = {
            {
                .binding = 0,
                .buffer = buffer,
                .offset = 0,
                .size = bd.size
            },
            {
                .binding = 1,
                .textureView = tv
            },
            {
                .binding = 2,
                .sampler = s,
            }
        };

        wgpu::BindGroupDescriptor group_descriptor {
            .layout = layout,
            .entryCount = 3,
            .entries = &group_entries[0]
        };
        return {
            vertex_count, 
            init.device.CreateBindGroup(&group_descriptor),
            buffer
        };
    }

    std::span<BlockData> GetRegistryBlockData() {
        auto& block_data_storage = registry->storage<BlockData>();
        if(!block_data_storage.contiguous())
            throw std::runtime_error("Block data buffer cannot be updated as the storage is not contiguous");

        return std::span<BlockData>(*block_data_storage.raw(), block_data_storage.size());
        // auto [_vertex_count, _group, _block_data_buffer] = CreateBufferAndBindGroup(Renderer::init, std::span<BlockData>(*block_data_storage.raw(), block_data_storage.size()), layout);
    }
}
