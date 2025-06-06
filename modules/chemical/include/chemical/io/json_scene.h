#pragma once

#include "chemical/scene.h"
namespace Chemical {
  extern std::optional<Scene> LoadSceneFromJSONFile(const std::string_view &file_path);
  inline void LoadSceneToJSONFile(const Scene &scene, const std::string_view &file_path) {};
} // namespace Chemical
