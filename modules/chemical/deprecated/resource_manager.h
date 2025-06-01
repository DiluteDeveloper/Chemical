#pragma once

#include "chemical/graphics/material.h"
#include "chemical/graphics/shader.h"
#include "chemical/graphics/texture.h"

#include <string_view>
#include <unordered_map>
namespace Chemical {

  namespace Graphics { class Renderer; }

  using ResourceFilePath = std::string;

  // loads resources from file into memory;
  // Intended to exist for the lifetime of application
  // resources in this class are essentially const on load
  // TODO: Make this class actually load resources from file
  // TODO: Split Acquire into Create and Get; having them one in the same
  // makes it hard to know when you have accidentally created a new object
  // instead of using an existing one
  class ResourceManager {
  public:
    // Get existing material if id exists, otherwise create and get
    Graphics::Material &AcquireMaterial(const ResourceFilePath &id);

    Graphics::Texture &
    CreateTexture(const Util::Image &image, const ResourceFilePath &id,
                  Graphics::Texture::ScalingFilter scaling_filter = Graphics::Texture::ScalingFilter::LINEAR);
    Graphics::Texture *GetTexture(const ResourceFilePath &id);

    Graphics::Shader *CreateShader(const Graphics::ShaderTraits &traits, const ResourceFilePath &id);
    Graphics::Shader *GetShader(const ResourceFilePath &id);

    std::string GetResourceFilePath(const ResourceFilePath &affix = "") const;

    ResourceManager(const std::string_view &resource_directory) : resource_directory(resource_directory) {}

  protected:
    friend class Graphics::Renderer;

    std::unordered_map<ResourceFilePath, Graphics::Material> materials;
    std::unordered_map<ResourceFilePath, Graphics::Texture> textures;
    std::unordered_map<ResourceFilePath, Graphics::Shader> shaders;

    const std::string resource_directory = "";
  };
} // namespace Chemical
