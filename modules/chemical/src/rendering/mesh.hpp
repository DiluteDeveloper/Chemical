#pragma once

#include "glm/glm.hpp"
#include <vector>

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
    glm::mat4 model = glm::mat4(1.0f);

    unsigned int AsVAO() const;
  };
} // namespace Chemical
