#pragma once

#include "gl_shader.h"
#include "rendering/renderer.h"

#include <unordered_map>

namespace Chemical {

  namespace OpenGL {

    class Renderer : public I_RRHandler, I_RenderingHandler {
    public:
      void RenderRenderables(const std::vector<Renderable>& renderables);

      bool RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits);
      bool RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits);

      Shader* GetShader(const std::string_view& shader_id);
      std::optional<uint32_t> GetTexture(const std::string_view& texture_id);

      Renderer();
      ~Renderer();

      Renderer(const Renderer& other) = delete;
      Renderer(Renderer&& other) = delete;
      Renderer& operator=(const Renderer&) = delete;
      Renderer& operator=(Renderer&&) = delete;

    protected:
      uint32_t bufferless_vao = 0;
      unsigned int frame_idx = 0;

      std::unordered_map<std::string, uint32_t> textures;
      std::unordered_map<std::string, Shader> shaders;
    };
  } // namespace OpenGL

} // namespace Chemical
