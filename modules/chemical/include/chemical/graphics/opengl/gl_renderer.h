#pragma once

#include "chemical/graphics/opengl/gl_material.h"
#include "chemical/graphics/opengl/gl_shader.h"

#include <string>
#include <unordered_map>

namespace Chemical {

  class Scene;

  class GL_Renderer {
  public:
    void RenderScene(Scene* scene);

    bool RegisterMaterial(const std::string_view& id, const MaterialTraits& traits);
    void DeregisterMaterial(const std::string_view& id);
    bool IsMaterial(const std::string_view& id);
    GL_Material* GetMaterial(const std::string_view& id);

    bool RegisterShader(const std::string_view& id, const ShaderTraits& traits);
    void DeregisterShader(const std::string_view& id);
    bool IsShader(const std::string_view& id);
    GL_Shader* GetShader(const std::string_view& id);

    GL_Renderer();
    ~GL_Renderer();

    GL_Renderer& operator=(GL_Renderer&&) = delete;
    GL_Renderer(GL_Renderer&&) = delete;
    GL_Renderer(const GL_Renderer&) = delete;
    GL_Renderer& operator=(const GL_Renderer&) = delete;

  protected:
    std::unordered_map<std::string, GL_Material> materials;
    std::unordered_map<std::string, GL_Shader> shaders;

    uint32_t bufferless_vao = 0;
  };
} // namespace Chemical
