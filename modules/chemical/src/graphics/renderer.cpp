#include "chemical/graphics/renderer.h"

#include "spdlog/spdlog.h"

namespace Chemical {
  namespace Graphics {

    Renderer::Renderer(ResourceManager &resources) : resources(resources) {

      ShaderTraits traits("default");

      traits.vs_file_path = resources.GetResourceFilePath("shaders/test_shader.vs");
      traits.fs_file_path = resources.GetResourceFilePath("shaders/test_shader.fs");

      resources.LoadShader(traits, "default");
    }

    void Renderer::RenderScene(const Scene &scene) {

      for (const auto &[shader_id, shader] : resources.shaders) {
        shader.Bind();

        for (const auto &mesh : scene.shader_mapped_static_meshes.at(shader_id)) {
          const Material &material = resources.materials.at(mesh.material_id);
          const Transform &transform = scene.transforms.at(mesh.transform_id);
          const Texture &texture = resources.textures.at(material.texture_id);

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
