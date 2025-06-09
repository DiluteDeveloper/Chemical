#pragma once

#include "chemical/graphics/material_traits.h"
#include "chemical/graphics/shader_traits.h"
#include "scene.h"

#include <vector>

namespace Chemical {

  template <typename T> using StrValVec = std::vector<std::pair<std::string, T>>;

  // - Loads resources from file into the application
  // - Responsible for all IO operations
  // - Does not store any resource data
  class ResourceManager {
  public:
    template <typename T> using OptionalObjectList = std::optional<std::vector<std::pair<std::string, T>>>;

    // resource_master_path is the suffix for all resource path parameters
    ResourceManager(const std::string_view& resource_master_path)
        : resource_master_path(resource_master_path) {}

    std::optional<Scene> LoadSceneJSONFile(const std::string_view& resource_path);

    std::optional<StrValVec<MaterialTraits>> LoadMaterialsJSONFile();

    std::optional<StrValVec<ShaderTraits>> LoadShadersJSONFile();

  protected:
    const std::string resource_master_path;
  };
} // namespace Chemical
