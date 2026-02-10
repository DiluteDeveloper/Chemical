#pragma once

#include <glm/glm.hpp>

namespace Chemical {

  namespace Maths {

    extern void SetPerlinNoiseSeed(unsigned int seed);
    extern double PerlinNoise3D(const glm::vec3 &val);

  } // namespace Maths
} // namespace Chemical
