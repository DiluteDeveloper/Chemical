#include "chemical/graphics/opengl/gl_renderer.h"

#include "chemical/scene.h"
#include "spdlog/spdlog.h"

#include <glad/glad.h>
namespace Chemical {

  GL_Renderer::GL_Renderer() {
    glCreateVertexArrays(1, &bufferless_vao);
  }
  void GL_Renderer::RenderScene(Scene* scene) {
    assert(scene != nullptr && "Scene is nullptr");

    for (const auto& [_, sprite] : scene->sprites) {

      GL_Material* material = GetMaterial(sprite.material_id);
      if (!material)
        throw std::runtime_error("Material is invalid");

      GL_Shader* shader = GetShader(material->shader_id);
      if (!shader)
        throw std::runtime_error("Shader is invalid");

      shader->Bind();
      shader->SetUniform3UIV("tint", 1, &material->tint[0]);

      shader->SetUniformMatrix3FV("v_model", 1, false,
                                  &scene->GetTransform(sprite.transform_id)->CreateMat3(sprite.size)[0][0]);

      glBindTextureUnit(0, material->texture_id);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  bool GL_Renderer::RegisterMaterial(const std::string_view& id, const MaterialTraits& traits) {
    auto [it, is_inserted] = materials.try_emplace(id.data());
    if (is_inserted)
      it->second.CreateGLMaterial(traits);
    return is_inserted;
  }
  void GL_Renderer::DeregisterMaterial(const std::string_view& id) {
    auto find = materials.find(id.data());
    if (find == materials.end())
      return;
    find->second.DeleteGLMaterial();

    materials.erase(id.data());
  }
  bool GL_Renderer::IsMaterial(const std::string_view& id) {
    return materials.contains(id.data());
  }
  GL_Material* GL_Renderer::GetMaterial(const std::string_view& id) {
    auto find = materials.find(id.data());
    if (find == materials.end()) {
      SPDLOG_WARN(R"(Tried to get material "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }
  bool GL_Renderer::RegisterShader(const std::string_view& id, const ShaderTraits& traits) {
    auto [it, is_inserted] = shaders.try_emplace(id.data());
    if (is_inserted) {
      GL_Shader& shader = it->second;
      shader.CreateGLShader(traits);
      glBindVertexArray(bufferless_vao);
      if (shader.compile_status == -1) {
        shader.DeleteGLShader();
        shaders.erase(id.data());
        return false;
      }
    }
    return is_inserted;
  }
  void GL_Renderer::DeregisterShader(const std::string_view& id) {
    auto find = shaders.find(id.data());
    if (find == shaders.end())
      return;
    find->second.DeleteGLShader();

    shaders.erase(id.data());
  }
  bool GL_Renderer::IsShader(const std::string_view& id) {
    return shaders.contains(id.data());
  }
  GL_Shader* GL_Renderer::GetShader(const std::string_view& id) {
    auto find = shaders.find(id.data());
    if (find == shaders.end()) {
      SPDLOG_WARN(R"(Tried to get shader "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }

  GL_Renderer::~GL_Renderer() {

    for (auto& [id, material] : materials) {
      material.DeleteGLMaterial();
    }
    for (auto& [id, shader] : shaders) {
      shader.DeleteGLShader();
    }
  }
} // namespace Chemical
