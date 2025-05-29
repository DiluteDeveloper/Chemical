#include "chemical/graphics/scene_renderer.h"

#include "chemical/resources.h"
#include "spdlog/spdlog.h"

namespace Chemical {
  namespace Graphics {

    SceneRenderer::SceneRenderer() {

      ShaderTraits default_shader("default");

      default_shader.vs_file_path = GetResourceDirectory("shaders/test_shader.vs");
      default_shader.fs_file_path = GetResourceDirectory("shaders/test_shader.fs");

      RegisterShader(default_shader, "default");
    }

    bool SceneRenderer::RegisterShader(const ShaderTraits &traits, const ObjectID &id) {

      Shader s(traits);
      if (s.GetCompileStatus() == 0) {
        spdlog::error("Shader failed to compile");
        return false;
      }
      shaders.emplace(id, std::move(s));

      return true;
    }

    void SceneRenderer::RenderScene(const Scene &scene) {

      for (const auto &[shader_id, shader] : shaders) {
        shader.Bind();

        for (const auto &mesh : scene.shader_mapped_static_meshes.at(shader_id)) {
          const Material &material = scene.materials.at(mesh.material_id);
          const Transform &transform = scene.transforms.at(mesh.transform_id);
          const Texture &texture = scene.textures.at(material.texture_id);

          texture.Bind();
          shader.SetUniform3F("colour", material.albedo.r / 255.0f, material.albedo.g / 255.0f,
                              material.albedo.b / 255.0f);
          shader.SetUniformMatrix3FV("v_model", 1, false, &transform.GetTransform()[0][0]);
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
      //   shader.SetUniform3F("colour", m.albedo.r / 255.0f, m.albedo.g / 255.0f, m.albedo.b / 255.0f);
      //
      //   shader.SetUniformMatrix3FV("v_model", 1, false, &mesh.transform.GetTransform()[0][0]);
      //   mesh.Draw();
      // }
    }

  } // namespace Graphics
} // namespace Chemical
