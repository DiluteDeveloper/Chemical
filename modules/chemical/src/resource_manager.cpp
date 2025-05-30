#include "chemical/resource_manager.h"

#include "spdlog/spdlog.h"

namespace Chemical {

  Graphics::Material *ResourceManager::LoadMaterial(const ResourceFilePath &id) { return &materials[id]; }

  Graphics::Material *ResourceManager::GetMaterial(const ResourceFilePath &id) { return &materials.at(id); }
  Graphics::Texture *ResourceManager::LoadTexture(const Graphics::TextureTraits &traits,
                                                  const ResourceFilePath &id) {
    return &textures.emplace(id, traits).first->second;
  }
  Graphics::Texture *ResourceManager::GetTexture(const ResourceFilePath &id) { return &textures.at(id); }

  Graphics::Shader *ResourceManager::LoadShader(const Graphics::ShaderTraits &traits,
                                                const ResourceFilePath &id) {

    Graphics::Shader s(traits);
    if (s.GetCompileStatus() == 0) {
      spdlog::error("Shader failed to compile");
      return nullptr;
    }
    return &shaders.emplace(id, std::move(s)).first->second;
  }
  Graphics::Shader *ResourceManager::GetShader(const ResourceFilePath &id) { return &shaders.at(id); }

  std::string ResourceManager::GetResourceFilePath(const ResourceFilePath &affix) {
    return std::string(resource_directory + affix);
  }
} // namespace Chemical
