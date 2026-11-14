#pragma once

#include <spdlog/spdlog.h>
#include <cstddef>
#include <cstring>
#include <glm/glm.hpp>
#include <webgpu/webgpu_cpp.h>

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

                r.emplace_back(d.start_position.x, d.start_position.y, d.start_position.z, 0);

                r.emplace_back((uint32_t)d.orientation, 
                        std::abs(d.end_position[arr[0]]) - std::abs(d.start_position[arr[0]]), 
                        std::abs(d.end_position[arr[1]]) - std::abs(d.start_position[arr[1]]), 0);
            }

            return r;
        }


        struct BlockBuffer {
            wgpu::Buffer buf;
            uint32_t vertex_count;
            wgpu::BindGroupLayout layout;
            wgpu::BindGroup group;
        };
        inline BlockBuffer InitialiseBlockBuffer(const wgpu::Device device) {

            std::vector<BlockData> raw = {
                {glm::ivec3(0, 0, 0), glm::ivec3(5, 0, 5), FaceOrientation::Bottom},
                {glm::ivec3(0, 5, 0), glm::ivec3(5, 5, 5), FaceOrientation::Top},
                {glm::ivec3(0, 0, 0), glm::ivec3(0, 5, 5), FaceOrientation::Left},
                {glm::ivec3(5, 0, 0), glm::ivec3(5, 5, 5), FaceOrientation::Right},
                {glm::ivec3(0, 0, 5), glm::ivec3(5, 5, 5), FaceOrientation::Front},
                {glm::ivec3(0, 0, 0), glm::ivec3(5, 5, 0), FaceOrientation::Back},
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


            wgpu::BindGroupLayoutEntry layout_entry {
                .binding = 0,
                .visibility = wgpu::ShaderStage::Vertex,
                .buffer = { nullptr, wgpu::BufferBindingType::ReadOnlyStorage, false, 0}
            };
            wgpu::BindGroupLayoutDescriptor layout_descriptor {
                .entryCount = 1,
                .entries = &layout_entry
            };
            r.layout = device.CreateBindGroupLayout(&layout_descriptor);
            wgpu::BindGroupEntry group_entry {
                .binding = 0,
                .buffer = r.buf,
                .offset = 0,
                .size = bd.size
            };
            wgpu::BindGroupDescriptor group_descriptor {
                .layout = r.layout,
                .entryCount = 1,
                .entries = &group_entry
            };
            r.group = device.CreateBindGroup(&group_descriptor);

            return r;
        }
    }
}
