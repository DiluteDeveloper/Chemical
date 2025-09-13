#pragma once

#include "assimp/Importer.hpp"
#include "rendering/material.hpp"
#include <cassert>
#include <vector>

#include "mesh.hpp"

namespace Chemical {

  struct Model {
    std::vector<Mesh> meshes;
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
