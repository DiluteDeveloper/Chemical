
#include "chemical/graphics/opengl/renderer.h"

#include "chemical/graphics/opengl/texture.h"
#include "spdlog/spdlog.h"

#include <glad/glad.h>

namespace Chemical {

  GL_Renderer::GL_Renderer() {
    glCreateVertexArrays(1, &bufferless_vao);
  }
  GL_Renderer::~GL_Renderer() {
    for (const auto& [id, texture] : textures) {
      glDeleteTextures(1, &texture);
    }
    for (const auto& [id, shader] : shaders) {
      shader.DeleteGLShader();
    }
  }
  void GL_Renderer::RenderScene(Scene& scene) const {

    for (const auto& [_, sprite] : scene.sprites) {

      const Material* material = scene.GetMaterial(sprite.material_id);
      if (!material) {
        SPDLOG_ERROR("Material is invalid");
        continue;
      }
      const Transform* transform = scene.GetTransform(sprite.transform_id);
      if (!transform) {
        SPDLOG_ERROR("transform is invalid");
        continue;
      }

      const auto f_shader = shaders.find(material->shader_id);
      if (f_shader == shaders.end()) {
        SPDLOG_ERROR("Shader is invalid");
        continue;
      }
      const GL_Shader shader = f_shader->second;

      const auto f_texture = textures.find(material->texture_id);
      if (f_texture == textures.end()) {
        SPDLOG_ERROR("Texture is invalid");
        continue;
      }
      const uint32_t texture = f_texture->second;

      shader.Bind();
      glBindVertexArray(bufferless_vao);
      shader.SetUniform3UIV("tint", 1, &material->tint[0]);

      shader.SetUniformMatrix3FV("v_model", 1, false, &transform->CreateMat3(sprite.size)[0][0]);

      glBindTextureUnit(0, texture);

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  bool GL_Renderer::RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits) {
    textures.try_emplace(texture_id.data(), GL_CreateTexture(traits));
    return true;
  }
  bool GL_Renderer::RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits) {

    if (!shaders.contains(shader_id.data())) {

      GL_Shader shader;
      shader.CreateGLShader(traits);
      if (shader.compile_status == -1) {
        shader.DeleteGLShader();
        return false;
      }
      shaders.emplace(shader_id.data(), std::move(shader));
      return true;
    }
    return false;
  }
} // namespace Chemical
