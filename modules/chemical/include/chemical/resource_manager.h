#pragma once

#include "chemical/graphics/renderer.h"
#include "scene.h"

#include <nlohmann-json/json.hpp>

using json = nlohmann::json;

namespace Chemical {

  // - Loads resources from file into the application
  // - Responsible for all IO operations
  // - Does not store any resource data
  class ResourceManager {
  public:
    ResourceManager() = delete;

    static void SetResourceMasterPath(const std::string_view& in_res_master_path);

    static bool LoadAndRegisterScene(const std::string_view& res_path, I_SORHandler& sor_handler);

    static bool LoadAndRegisterProject(const std::string_view& res_path, I_RRHandler& rr_handler);

  protected:
    static std::optional<json> LoadJSONFile(const std::string_view& path);
  };
} // namespace Chemical
