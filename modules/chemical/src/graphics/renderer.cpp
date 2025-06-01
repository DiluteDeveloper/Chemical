#include "chemical/graphics/renderer.h"

#include "spdlog/spdlog.h"

namespace Chemical {
  namespace Graphics {

    void Renderer::RenderScene(Scene &scene) {

      for (const auto &[shader_id, shader] : scene.shaders) {
        shader.Bind();

        if (!scene.shader_mapped_static_meshes.contains(shader_id))
          continue;

        for (const auto &mesh : scene.shader_mapped_static_meshes.at(shader_id)) {
          const Transform *transform = scene.GetTransform(mesh.transform_id);

          if (transform == nullptr) {
            SPDLOG_ERROR(R"(Failed to render static mesh: transform "{}" does not exist!)",
                         mesh.transform_id);
            continue;
          }

          const Material *material = scene.GetMaterial(mesh.material_id);
          if (material == nullptr) {
            SPDLOG_ERROR(R"(Failed to render static mesh: material "{}" does not exist!)", mesh.material_id);

            continue;
          }
          const Texture *texture = scene.GetTexture(material->texture_id);
          if (texture == nullptr) {
            SPDLOG_ERROR(R"(Failed to render static mesh: texture "{}" does not exist!)",
                         material->texture_id);
            continue;
          }

          texture->Bind();
          shader.SetUniform3F("colour", material->tint.r / 255.0f, material->tint.g / 255.0f,
                              material->tint.b / 255.0f);
          shader.SetUniformMatrix3FV("v_model", 1, false, &transform->GetTransform()[0][0]);
          mesh.Draw();
        }
      }
      //
      // for (auto &[_, mesh] : dynamic_meshes) {
      //
      //   const Shader &shader = at(mesh.shader_id).shader;
      //   shader.Bind();
      //   const Material &m = materials.at(mesh.material_id);
      //   textures.at(m.texture_id).Bind();
      //   shader.SetUniform3F("colour", m.tint.r / 255.0f, m.tint.g / 255.0f, m.tint.b / 255.0f);
      //
      //   shader.SetUniformMatrix3FV("v_model", 1, false, &mesh.transform.GetTransform()[0][0]);
      //   mesh.Draw();
      // }
    }

  } // namespace Graphics
} // namespace Chemical
