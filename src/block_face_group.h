#pragma once

#include <glm/glm.hpp>
#include "world/block.h"
#include "block_type.h"

namespace Chemical {

    struct BlockFaceGroup {
        glm::ivec3 start_position;
        glm::ivec3 end_position;
        BlockType block_type;
        BlockFaceOrientation block_face_orientation;

    };
}
