#pragma once

#include <string>
#include <vector>

#include "material.hpp"
#include "util/transform.hpp"

namespace Chemical {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;

    Vertex(glm::vec3 position = glm::vec3(1.0f),
           glm::vec3 normal = glm::vec3(0.0f))
        : position(position), normal(normal) {}
  };
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Transform transform;
    std::string name = "default";
    Material material;

    unsigned int AsVAO() const;
  };
} // namespace Chemical
