#pragma once

#include <glm/glm.hpp>

namespace Chemical {

    namespace Block {

        enum class Type:uint8_t { Dirt, Grass };
        enum class FaceOrientation:uint8_t { TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK };

        struct FaceGroup { 
            glm::ivec3 start_position; 
            glm::ivec3 end_position; 
            Type type; 
            FaceOrientation face_orientation;
        };
    
    }
}
