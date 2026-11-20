#pragma once

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
        glm::ivec3 start_position; 
        glm::ivec3 end_position; 
        FaceOrientation orientation;
        Type type;

    };
    inline std::vector<BlockData> GetTestBlockData() {
        return {
            {glm::ivec3(0, 0, 0), glm::ivec3(5, 0, 5), FaceOrientation::Bottom, Type::Stone},
            {glm::ivec3(0, 7, 0), glm::ivec3(5, 7, 5), FaceOrientation::Top, Type::Grass},

            {glm::ivec3(0, 0, 0), glm::ivec3(0, 3, 5), FaceOrientation::Left, Type::Stone},
            {glm::ivec3(0, 3, 0), glm::ivec3(0, 4, 5), FaceOrientation::Left, Type::StoneDirt},
            {glm::ivec3(0, 4, 0), glm::ivec3(0, 6, 5), FaceOrientation::Left, Type::Dirt},
            {glm::ivec3(0, 6, 0), glm::ivec3(0, 7, 5), FaceOrientation::Left, Type::GrassDirt},
            {glm::ivec3(5, 0, 0), glm::ivec3(5, 3, 5), FaceOrientation::Right, Type::Stone},
            {glm::ivec3(5, 3, 0), glm::ivec3(5, 4, 5), FaceOrientation::Right, Type::StoneDirt},
            {glm::ivec3(5, 4, 0), glm::ivec3(5, 6, 5), FaceOrientation::Right, Type::Dirt},
            {glm::ivec3(5, 6, 0), glm::ivec3(5, 7, 5), FaceOrientation::Right, Type::GrassDirt},

            {glm::ivec3(0, 0, 5), glm::ivec3(5, 3, 5), FaceOrientation::Front, Type::Stone},
            {glm::ivec3(0, 3, 5), glm::ivec3(5, 4, 5), FaceOrientation::Front, Type::StoneDirt},
            {glm::ivec3(0, 4, 5), glm::ivec3(5, 6, 5), FaceOrientation::Front, Type::Dirt},
            {glm::ivec3(0, 6, 5), glm::ivec3(5, 7, 5), FaceOrientation::Front, Type::GrassDirt},

            {glm::ivec3(0, 0, 0), glm::ivec3(5, 3, 0), FaceOrientation::Back, Type::Stone},
            {glm::ivec3(0, 3, 0), glm::ivec3(5, 4, 0), FaceOrientation::Back, Type::StoneDirt},
            {glm::ivec3(0, 4, 0), glm::ivec3(5, 6, 0), FaceOrientation::Back, Type::Dirt},
            {glm::ivec3(0, 6, 0), glm::ivec3(5, 7, 0), FaceOrientation::Back, Type::GrassDirt},

            {glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 0+2, 5+2), FaceOrientation::Bottom, Type::Stone},
            {glm::ivec3(0+8, 7+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Top, Type::Grass},

            {glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(0+8, 3+2, 5+2), FaceOrientation::Left, Type::Stone},
            {glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(0+8, 4+2, 5+2), FaceOrientation::Left, Type::StoneDirt},
            {glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(0+8, 6+2, 5+2), FaceOrientation::Left, Type::Dirt},
            {glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(0+8, 7+2, 5+2), FaceOrientation::Left, Type::GrassDirt},
            {glm::ivec3(5+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Right, Type::Stone},
            {glm::ivec3(5+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Right, Type::StoneDirt},
            {glm::ivec3(5+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Right, Type::Dirt},
            {glm::ivec3(5+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Right, Type::GrassDirt},

            {glm::ivec3(0+8, 0+2, 5+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Front, Type::Stone},
            {glm::ivec3(0+8, 3+2, 5+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Front, Type::StoneDirt},
            {glm::ivec3(0+8, 4+2, 5+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Front, Type::Dirt},
            {glm::ivec3(0+8, 6+2, 5+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Front, Type::GrassDirt},

            {glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 0+2), FaceOrientation::Back, Type::Stone},
            {glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 0+2), FaceOrientation::Back, Type::StoneDirt},
            {glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 0+2), FaceOrientation::Back, Type::Dirt},
            {glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 0+2), FaceOrientation::Back, Type::GrassDirt},

            {glm::ivec3(-40, 4, -40), glm::ivec3(40, 4, 40), FaceOrientation::Top, Type::Dirt},
            // {glm::ivec3(0+0, 0+8, 0+2), glm::ivec3(5+0, 0+8, 5+2), FaceOrientation::Bottom, Type::Glass},
            // {glm::ivec3(0+0, 7+8, 0+2), glm::ivec3(5+0, 7+8, 5+2), FaceOrientation::Top, Type::Glass},
            //
            // {glm::ivec3(0+0, 0+8, 0+2), glm::ivec3(0+0, 7+8, 5+2), FaceOrientation::Left, Type::Glass},
            // {glm::ivec3(5+0, 0+8, 0+2), glm::ivec3(5+0, 7+8, 5+2), FaceOrientation::Right, Type::Glass},
            // {glm::ivec3(0+0, 0+8, 5+2), glm::ivec3(5+0, 7+8, 5+2), FaceOrientation::Front, Type::Glass},
            // {glm::ivec3(0+0, 0+8, 0+2), glm::ivec3(5+0, 7+8, 0+2), FaceOrientation::Back, Type::Glass},

            {glm::ivec3(5, 5, 5), glm::ivec3(5, 20, 20), FaceOrientation::Left, Type::Glass},
            {glm::ivec3(20, 5, 5), glm::ivec3(20, 20, 20), FaceOrientation::Right, Type::Glass},
            {glm::ivec3(5, 5, 20), glm::ivec3(20, 20, 20), FaceOrientation::Front, Type::Glass},
            {glm::ivec3(5, 5, 5), glm::ivec3(20, 20, 5), FaceOrientation::Back, Type::Glass},
            {glm::ivec3(5, 20, 5), glm::ivec3(20, 20, 20), FaceOrientation::Top, Type::Glass},
            {glm::ivec3(5, 5, 5), glm::ivec3(20, 5, 20), FaceOrientation::Bottom, Type::Glass},
        };
    }

}
