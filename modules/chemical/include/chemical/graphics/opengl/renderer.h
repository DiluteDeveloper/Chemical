#pragma once

#include "chemical/graphics/opengl/shader.h"
#include "chemical/graphics/renderer.h"

namespace Chemical {

  class GL_Renderer : public I_RRHandler, I_RSHandler {
  public:
    void RenderScene(Scene& scene) const;

    bool RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits);
    bool RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits);

    GL_Renderer();
    ~GL_Renderer();

    GL_Renderer(const GL_Renderer& other) = delete;
    GL_Renderer(GL_Renderer&& other) = delete;
    GL_Renderer& operator=(const GL_Renderer&) = delete;
    GL_Renderer& operator=(GL_Renderer&&) = delete;

  protected:
    uint32_t bufferless_vao = 0;

    std::unordered_map<std::string, uint32_t> textures;
    std::unordered_map<std::string, GL_Shader> shaders;
  };
} // namespace Chemical
