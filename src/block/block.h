#pragma once

#include <stb_image.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <webgpu/webgpu_cpp.h>

#include "utils/image.h"

namespace Chemical {

    namespace Block {

        // enum class Type:uint8_t { Dirt, Grass };
        enum class FaceOrientation:uint8_t { 
            Top = 0, 
            Bottom = 1, 
            Left = 2, 
            Right = 3, 
            Front = 4, 
            Back = 5
        };
        enum class Type:uint8_t {
            Dirt,
            Stone
        };
        //
        // struct FaceGroup { 
        //     glm::ivec3 start_position; 
        //     glm::ivec3 end_position; 
        //     Type type; 
        //     FaceOrientation face_orientation;
        // };
    
        struct BlockData {
            glm::ivec3 start_position; 
            glm::ivec3 end_position; 
            FaceOrientation orientation;
            Type type;

        };
        inline std::vector<glm::ivec4> BlockFaceToShaderData(const std::vector<BlockData>& data) {
			static constexpr std::array<std::array<uint8_t, 2>, 6> face_orientation_to_position_dimension_idx = {{
				{0, 2}, // FaceOrientation::Top
				{2, 0}, // FaceOrientation::Bottom
				{1, 2}, // FaceOrientation::Left
				{2, 1}, // FaceOrientation::Right
				{1, 0}, // FaceOrientation::Front
				{0, 1} // FaceOrientation::Back
			}};

            std::vector<glm::ivec4> r;
            for(auto& d : data) {
                auto& arr = face_orientation_to_position_dimension_idx[(uint8_t)d.orientation];

                r.emplace_back(d.start_position.x, d.start_position.y, d.start_position.z, d.type);

                r.emplace_back((uint32_t)d.orientation, 
                        std::abs(d.end_position[arr[0]]) - std::abs(d.start_position[arr[0]]), 
                        std::abs(d.end_position[arr[1]]) - std::abs(d.start_position[arr[1]]), 0);
            }

            return r;
        }

        std::tuple<wgpu::Texture, wgpu::TextureDescriptor> SetupTexture(wgpu::Device device, wgpu::Queue queue);
        struct BlockBuffer {
            wgpu::Buffer buf;
            uint32_t vertex_count;
            wgpu::BindGroupLayout layout;
            wgpu::BindGroup group;
        };
        inline BlockBuffer InitialiseBlockBuffer(wgpu::Device device, wgpu::Queue queue) {

            std::vector<BlockData> raw = {
                {glm::ivec3(0, 0, 0), glm::ivec3(5, 0, 5), FaceOrientation::Bottom, Type::Dirt},
                {glm::ivec3(0, 5, 0), glm::ivec3(5, 5, 5), FaceOrientation::Top, Type::Stone},
                {glm::ivec3(0, 0, 0), glm::ivec3(0, 5, 5), FaceOrientation::Left, Type::Dirt},
                {glm::ivec3(5, 0, 0), glm::ivec3(5, 5, 5), FaceOrientation::Right, Type::Stone},
                {glm::ivec3(0, 0, 5), glm::ivec3(5, 5, 5), FaceOrientation::Front, Type::Dirt},
                {glm::ivec3(0, 0, 0), glm::ivec3(5, 5, 0), FaceOrientation::Back, Type::Stone},
            };
            auto data = BlockFaceToShaderData(raw);

            wgpu::BufferDescriptor bd;
            bd.size = data.size() * sizeof(glm::ivec4);
            bd.usage = wgpu::BufferUsage::Storage;
            bd.mappedAtCreation = true;

            BlockBuffer r = {
                .buf = device.CreateBuffer(&bd),
                .vertex_count = (uint32_t)data.size() * 3,
                .layout = {},
                .group = {}
            };

           
            r.buf.WriteMappedRange(0, data.data(), bd.size);

            r.buf.Unmap();

            auto[texture, texture_desc] = SetupTexture(device, queue);
            wgpu::TextureViewDescriptor texture_view_desc = {
                .format = texture_desc.format,
                .dimension = wgpu::TextureViewDimension::e2DArray,
                .mipLevelCount = 1,
                .arrayLayerCount = 2,
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

            wgpu::Sampler s = device.CreateSampler(&sd);

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
            r.layout = device.CreateBindGroupLayout(&layout_descriptor);
            wgpu::BindGroupEntry group_entries[] = {
                {
                    .binding = 0,
                    .buffer = r.buf,
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
                .layout = r.layout,
                .entryCount = 3,
                .entries = &group_entries[0]
            };
            r.group = device.CreateBindGroup(&group_descriptor);



            return r;
        }
        inline std::tuple<wgpu::Texture, wgpu::TextureDescriptor> SetupTexture(wgpu::Device device, wgpu::Queue queue) {
            

            auto image = Utils::Image::LoadImage("res/textures/blocks/atlas.png", 4);
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
                .format = wgpu::TextureFormat::RGBA8UnormSrgb,
                .mipLevelCount = 1,
                .sampleCount = 1,
            };

            wgpu::Texture texture = device.CreateTexture(&desc);

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

                    layout.offset = ix*unit_byte_offset + (iy*atlas_units_x*unit_byte_offset);
                    destination.origin.z = ix + (iy*atlas_units_x);
                    queue.WriteTexture(&destination, image->data, image_byte_size, &layout, &extent);
                }
            }

            // wgpu::Extent3D x1 = {32, 32, 1};
            // queue.WriteTexture(&destination, i.data, 64 * 64 * i.num_channels, &layout, &x1);
            // layout.offset = 4*32;
            // destination.origin.z = 1;
            // queue.WriteTexture(&destination, i.data, 64 * 64 * i.num_channels, &layout, &x1);
            // x1.depthOrArrayLayers = 2;
            // queue.WriteTexture(&destination, i.data, 32 * 64 * i.num_channels, &layout, &x1);
            // wgpu::Extent3D x2 = {32, 32, 2};
            // queue.WriteTexture(&destination, i.data + (32 * 4), 32 * 32 * i.num_channels, &layout, &x2);

            return std::make_tuple(texture, desc);
        }
    }

}
