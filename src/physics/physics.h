#pragma once

#include "utils/transform.h"

namespace Chemical {
    Transform ApplyVelocityToTransform(const glm::vec3& velocity, const Transform& transform);
}

