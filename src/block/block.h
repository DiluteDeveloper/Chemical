#pragma once

#include <endian.h>
#include <glm/glm.hpp>
#include <webgpu/webgpu_cpp.h>

namespace Chemical {

    namespace Block {

        // enum class Type:uint8_t { Dirt, Grass };
        // enum class FaceOrientation:uint8_t { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };
        //
        // struct FaceGroup { 
        //     glm::ivec3 start_position; 
        //     glm::ivec3 end_position; 
        //     Type type; 
        //     FaceOrientation face_orientation;
        // };
    
        struct BlockBuffer {
            wgpu::Buffer buf;
            uint32_t vertex_count;
            wgpu::BindGroupLayout layout;
            wgpu::BindGroup group;
        };
        inline BlockBuffer InitialiseBlockBuffer(const wgpu::Device device) {

            std::vector<glm::fvec4> block_data = { {-0.5, -0.5, 1.0, 0.0}, {0.5, 0.5, 5.0, 0.0} };
            
            wgpu::BufferDescriptor bd;
            bd.size = block_data.size() * sizeof(glm::fvec4);
            bd.usage = wgpu::BufferUsage::Storage;
            bd.mappedAtCreation = true;

            BlockBuffer r = {
                .buf = device.CreateBuffer(&bd),
                .vertex_count = (uint32_t)(block_data.size() * 3),
                .layout = {},
                .group = {}
            };
            
            r.buf.WriteMappedRange(0, block_data.data(), bd.size);

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
