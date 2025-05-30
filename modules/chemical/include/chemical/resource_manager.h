#pragma once

#include "chemical/graphics/material.h"
#include "chemical/graphics/shader.h"
#include "chemical/graphics/texture.h"

#include <string_view>
#include <unordered_map>
namespace Chemical {

  namespace Graphics {
    class Renderer;
  }

  using ResourceFilePath = std::string;

  // Loads resources from file into memory;
  // Intended to exist for the lifetime of application
  // resources in this class are essentially const on load
  // TODO: Make this class actually load assets from file
  class ResourceManager {
  public:
    Graphics::Material *LoadMaterial(const ResourceFilePath &id);
    Graphics::Material *GetMaterial(const ResourceFilePath &id);

    Graphics::Texture *LoadTexture(const Graphics::TextureTraits &traits, const ResourceFilePath &id);
    Graphics::Texture *GetTexture(const ResourceFilePath &id);

    Graphics::Shader *LoadShader(const Graphics::ShaderTraits &traits, const ResourceFilePath &id);
    Graphics::Shader *GetShader(const ResourceFilePath &id);

    std::string GetResourceFilePath(const ResourceFilePath &affix = "");

    ResourceManager(const std::string_view &resource_directory) : resource_directory(resource_directory) {}

  protected:
    friend class Graphics::Renderer;

    std::unordered_map<ResourceFilePath, Graphics::Material> materials;
    std::unordered_map<ResourceFilePath, Graphics::Texture> textures;
    std::unordered_map<ResourceFilePath, Graphics::Shader> shaders;

    const std::string resource_directory = "";
  };
} // namespace Chemical
