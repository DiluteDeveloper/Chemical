
#include "rendering/opengl/gl_renderer.h"

#include "rendering/opengl/gl_texture.h"
#include "spdlog/spdlog.h"

#include <glad/glad.h>

namespace Chemical {

  namespace OpenGL {

    Renderer::Renderer() {
      glCreateVertexArrays(1, &bufferless_vao);
    }
    Renderer::~Renderer() {
      for (const auto& [id, texture] : textures) {
        glDeleteTextures(1, &texture);
      }
      for (const auto& [id, shader] : shaders) {
        shader.DeleteGLShader();
      }
    }
    Shader* Renderer::GetShader(const std::string_view& shader_id) {
      auto find = shaders.find(shader_id.data());
      if (find == shaders.end()) {
        SPDLOG_WARN(R"(Tried to get shader "{}" that does not exist)", shader_id);
        return nullptr;
      }
      return &find->second;
    }
    std::optional<uint32_t> Renderer::GetTexture(const std::string_view& texture_id) {
      auto find = textures.find(texture_id.data());
      if (find == textures.end()) {
        SPDLOG_WARN(R"(Tried to get texture "{}" that does not exist)", texture_id);
        return std::nullopt;
      }
      return find->second;
    }
    void Renderer::RenderRenderables(const std::vector<Renderable>& renderables) {
      for (const auto& renderable : renderables) {
        const Shader* shader = GetShader(renderable.shader_id);
        if (!shader)
          continue;
        std::optional<uint32_t> texture = GetTexture(renderable.texture_id);
        if (!texture)
          continue;

        shader->Bind();
        glBindVertexArray(bufferless_vao);
        shader->SetUniform3UIV("tint", 1, &renderable.tint[0]);

        shader->SetUniformMatrix3FV("v_model", 1, false, &renderable.model[0][0]);
        shader->SetUniformMatrix3FV("v_view", 1, false, &renderable.view[0][0]);
        shader->SetUniformMatrix3FV("v_proj", 1, false, &renderable.proj[0][0]);
        shader->SetUniform1UI("v_frame_idx", frame_idx);

        glBindTextureUnit(0, *texture);

        glDrawArrays(GL_TRIANGLES, 0, 6);
      }
      frame_idx++;
    }
    // void Renderer::RenderScene(Scene& scene) const {
    //
    //   for (const auto& [_, sprite] : scene.sprites) {
    //
    //     const Material* material = scene.GetMaterial(sprite.material_id);
    //     if (!material) {
    //       SPDLOG_ERROR("Material is invalid");
    //       continue;
    //     }
    //     const Transform* transform = scene.GetTransform(sprite.transform_id);
    //     if (!transform) {
    //       SPDLOG_ERROR("transform is invalid");
    //       continue;
    //     }
    //
    //     const auto f_shader = shaders.find(material->shader_id);
    //     if (f_shader == shaders.end()) {
    //       SPDLOG_ERROR("Shader is invalid");
    //       continue;
    //     }
    //     const GL_Shader shader = f_shader->second;
    //
    //     const auto f_texture = textures.find(material->texture_id);
    //     if (f_texture == textures.end()) {
    //       SPDLOG_ERROR("Texture is invalid");
    //       continue;
    //     }
    //     const uint32_t texture = f_texture->second;
    //
    //     shader.Bind();
    //     glBindVertexArray(bufferless_vao);
    //     shader.SetUniform3UIV("tint", 1, &material->tint[0]);
    //
    //     shader.SetUniformMatrix3FV("v_model", 1, false, &transform->CreateMat3(sprite.size)[0][0]);
    //
    //     glBindTextureUnit(0, texture);
    //
    //     glDrawArrays(GL_TRIANGLES, 0, 6);
    //   }
    // }

    bool Renderer::RegisterTexture(const std::string_view& texture_id, const TextureTraits& traits) {
      textures.try_emplace(texture_id.data(), CreateTexture(traits));
      return true;
    }
    bool Renderer::RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits) {

      if (!shaders.contains(shader_id.data())) {

        Shader shader;
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

  } // namespace OpenGL

} // namespace Chemical
