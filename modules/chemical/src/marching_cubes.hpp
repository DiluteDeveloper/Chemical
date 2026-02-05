#pragma once

#include "rendering/mesh.hpp"
#include "terrain_state.hpp"

namespace Chemical {

  extern Mesh GenerateMarchingCubes(const Util::IsoGrid &noise_data,
                                    float iso_value);
}
