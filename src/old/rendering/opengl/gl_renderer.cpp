
#include "rendering/opengl/gl_renderer.h"

#include "rendering/opengl/gl_texture.h"
#include "spdlog/spdlog.h"

#include <GLES3/gl3.h>

namespace Chemical {

  namespace OpenGL {

    Renderer::Renderer() {
      glGenVertexArrays(1, &bufferless_vao);
    }
    Renderer::~Renderer() {
      for (const auto& [id, texture] : textures) {
        glDeleteTextures(1, &texture);
      }
      for (const auto& [id, shader] : shaders) {
        shader.DeleteGLShader();
      }
      glDeleteVertexArrays(1, &bufferless_vao);
    }
    Shader* Renderer::GetShader(const std::string_view& shader_id) {
      auto find = shaders.find(std::string(shader_id));
      if (find == shaders.end()) {
        SPDLOG_WARN(R"(Tried to get shader "{}" that does not exist)", shader_id);
        return nullptr;
      }
      return &find->second;
    }
    std::optional<uint32_t> Renderer::GetTexture(const std::string_view& texture_id) {
      auto find = textures.find(std::string(texture_id));
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

        // The shader must be bound before its uniforms are set (no
        // glProgramUniform* in ES 3.0).
        shader->Bind();
        glBindVertexArray(bufferless_vao);
        shader->SetUniform3UIV("tint", 1, &renderable.tint[0]);

        shader->SetUniformMatrix3FV("v_model", 1, false, &renderable.model[0][0]);
        shader->SetUniformMatrix3FV("v_view", 1, false, &renderable.view[0][0]);
        shader->SetUniformMatrix3FV("v_proj", 1, false, &renderable.proj[0][0]);
        shader->SetUniform1UI("v_frame_idx", frame_idx);

        // Replaces glBindTextureUnit(0, texture). The sampler uniform defaults
        // to unit 0, so it does not need to be set.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, *texture);

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
      const std::string key(texture_id);
      // Check first so an existing id does not create (and leak) a second GL texture.
      if (textures.contains(key)) {
        return false;
      }
      textures.emplace(key, CreateTexture(traits));
      return true;
    }
    bool Renderer::RegisterShader(const std::string_view& shader_id, const ShaderTraits& traits) {

      const std::string key(shader_id);
      if (shaders.contains(key)) {
        return false;
      }

      Shader shader;
      shader.CreateGLShader(traits);
      if (shader.compile_status == -1) {
        shader.DeleteGLShader();
        return false;
      }
      shaders.emplace(key, std::move(shader));
      return true;
    }

  } // namespace OpenGL

} // namespace Chemical
