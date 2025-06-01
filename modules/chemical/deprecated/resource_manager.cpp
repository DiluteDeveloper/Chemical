#include "chemical/resource_manager.h"

#include "spdlog/spdlog.h"

#include <tuple>
#include <utility>

namespace Chemical {

  Graphics::Material &ResourceManager::AcquireMaterial(const ResourceFilePath &id) {

    return materials[id];
  }

  Graphics::Texture &ResourceManager::CreateTexture(const Util::Image &image, const ResourceFilePath &id,
                                                    Graphics::Texture::ScalingFilter scaling_filter) {
    auto it = textures.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                               std::forward_as_tuple(image, scaling_filter));

    SPDLOG_INFO(R"(Creating texture "{}")", id);
    if (!it.second)
      SPDLOG_WARN(
          R"(Could not create texture "{}" : texture already exists with that ID, returning existing texture)",
          id);
    return it.first->second;
  }
  Graphics::Texture *ResourceManager::GetTexture(const ResourceFilePath &id) {

    auto find = textures.find(id);
    if (find == textures.end()) {
      SPDLOG_WARN(R"(Could not get texture "{}" : texture with that ID does not exist, returning nullptr)",
                  id);
      return nullptr;
    }
    return &find->second;
  }

  Graphics::Shader *ResourceManager::CreateShader(const Graphics::ShaderTraits &traits,
                                                  const ResourceFilePath &id) {

    SPDLOG_INFO(R"(Creating shader "{}")", id);
    Graphics::Shader s(traits);
    if (s.compile_status == -1) {
      SPDLOG_ERROR(R"(Failed to create shader "{}" : Shader failed to compile, returning nullptr)", id);
      return nullptr;
    }
    return &shaders.emplace(id, std::move(s)).first->second;
  }
  Graphics::Shader *ResourceManager::GetShader(const ResourceFilePath &id) {

    auto find = shaders.find(id);
    if (find == shaders.end()) {
      SPDLOG_WARN(R"(Could not get shader "{}" : shader with that ID does not exist, returning nullptr)", id);
      return nullptr;
    }
    return &find->second;
  }

  std::string ResourceManager::GetResourceFilePath(const ResourceFilePath &affix) const {
    return std::string(resource_directory + affix);
  }
} // namespace Chemical
