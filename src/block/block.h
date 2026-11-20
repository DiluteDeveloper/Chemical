#pragma once

#include <entt/entity/fwd.hpp>
#include <stb_image.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <webgpu/webgpu_cpp.h>

namespace Chemical {

    // enum class Type:uint8_t { Dirt, Grass };
    enum class FaceOrientation:uint8_t { 
        Top = 0, 
        Bottom = 1, 
        Right = 2, 
        Left = 3, 
        Back = 4,
        Front = 5 
    };
    enum class Type:uint8_t {
        Grass = 0,
        Dirt = 1,
        GrassDirt = 2,
        Stone = 3,
        StoneDirt = 4,
        TEST_ARROW = 5,
        Glass = 6
    };
    struct BlockData {
        static void on_construct(entt::registry&, const entt::entity);
        static void on_update(entt::registry&, const entt::entity);

        glm::ivec3 start_position; 
        glm::ivec3 end_position; 
        FaceOrientation orientation;
        Type type;

    };
    extern void AddTestBlocksToRegistry(entt::registry&);

}
