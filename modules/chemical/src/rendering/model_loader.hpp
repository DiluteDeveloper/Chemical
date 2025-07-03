#pragma once

#include "assimp/Importer.hpp"
#include "rendering/material.hpp"
#include <cassert>
#include <vector>

namespace Chemical {

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
  };
  struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
  };
  struct Model {
    Mesh mesh;
    Material material;
  };

  // Static utility class for loading models using assimp.
  // Stores a Assimp::Importer instance.
  class ModelLoader {
  public:
    static std::optional<Model> LoadModel(const std::string_view &file_path);

  protected:
    static Assimp::Importer importer;
  };
} // namespace Chemical
