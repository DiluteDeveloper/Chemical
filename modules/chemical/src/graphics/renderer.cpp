#include "chemical/graphics/renderer.h"

#include "spdlog/spdlog.h"

namespace Chemical {
  namespace Graphics {

    void Renderer::RenderScene(Scene &scene) {

      for (const auto &[shader_id, shader] : scene.shaders) {
        shader.Bind();

        if (!scene.shader_mapped_static_meshes.contains(shader_id))
          continue;

        Camera *camera = scene.GetCamera(scene.primary_camera);

        if (camera == nullptr)
          SPDLOG_ERROR(R"(Failed to render: primary camera "{}" does not exist!)", scene.primary_camera);

        Transform *camera_transform = scene.GetTransform(camera->transform_id);

        if (camera_transform == nullptr)
          SPDLOG_ERROR(R"(Failed to render: primary camera transform "{}" does not exist!)",
                       camera->transform_id);

        shader.SetUniformMatrix3FV("v_proj", 1, false, &camera->projection_matrix[0][0]);
        shader.SetUniformMatrix3FV("v_view", 1, false, &glm::inverse(camera_transform->CreateMat3())[0][0]);

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
          shader.SetUniformMatrix3FV("v_model", 1, false, &transform->CreateMat3()[0][0]);
          mesh.Draw();
        }
      }
    }

  } // namespace Graphics
} // namespace Chemical
