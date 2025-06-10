#pragma once

#include "chemical/graphics/shader.h"
#include "chemical/graphics/texture.h"
#include "chemical/scene.h"

namespace Chemical {

  // IRendererRegistrationHandler
  struct I_RRHandler {
    virtual bool RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits) = 0;
    virtual bool RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits) = 0;
  };

  // IRendererSceneHandler
  struct I_RSHandler {
    virtual void RenderScene(Scene& scene) const = 0;
  };
} // namespace Chemical
