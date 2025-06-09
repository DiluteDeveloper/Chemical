#pragma once

#include "chemical/graphics/material_traits.h"

namespace Chemical {

  // Factory pattern is more explicit for OpenGL functionality
  class GL_Material {
  public:
    GL_Material() = default;

    TextureFilter texture_filter;
    glm::uvec3 tint = glm::uvec3(255);
    std::string shader_id = "default";

  protected:
    friend class GL_Renderer;
    uint32_t texture_id = 0;

    void CreateGLMaterial(const MaterialTraits& traits);
    void DeleteGLMaterial();
  };
} // namespace Chemical
