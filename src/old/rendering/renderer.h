#pragma once

#include "rendering/renderable.h"
#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <vector>

namespace Chemical {

  // IRendererRegistrationHandler
  struct I_RRHandler {
    virtual bool RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits) = 0;
    virtual bool RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits) = 0;
  };

  struct I_RenderingHandler {
    virtual void RenderRenderables(const std::vector<Renderable>& renderables) = 0;
  };
} // namespace Chemical
