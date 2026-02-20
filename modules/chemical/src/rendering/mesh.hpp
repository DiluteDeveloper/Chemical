#pragma once

#include <string>
#include <vector>

#include "material.hpp"

namespace Chemical {

  struct Vertex {
    glm::fvec3 position;
    glm::fvec3 normal;

    constexpr Vertex(glm::fvec3 position = glm::fvec3(1.0f),
                     glm::fvec3 normal = glm::fvec3(0.0f))
        : position(position), normal(normal) {}
  };
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string name = "default";
    Material material;

    unsigned int AsVAO();
  };
} // namespace Chemical
