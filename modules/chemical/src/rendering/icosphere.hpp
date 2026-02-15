#pragma once

#include "rendering/mesh.hpp"
namespace Chemical {

  extern Mesh GenerateIcosphereFlatNormals(unsigned int resolution);
  extern Mesh GenerateIcosphereSmoothNormals(unsigned int resolution);
} // namespace Chemical
