#include "chemical/graphics/renderer.h"

#include "chemical/resources.h"

#include <spdlog/spdlog.h>
#include <tuple>
#include <utility>

namespace Chemical {
  namespace Graphics {

    Renderer::Renderer() {

      spdlog::info("Initialising and registering default 2D shader");
      // Create and register default 2D shader
      ShaderTraits default_shader("default");

      default_shader.vs_file_path = GetResourceDirectory("shaders/test_shader.vs");
      default_shader.fs_file_path = GetResourceDirectory("shaders/test_shader.fs");

      RegisterShader(default_shader);

      Material default_material;
      RegisterMaterial(default_material, "default");

      TextureTraits default_texture(GetResourceDirectory("textures/1x1.png"));

      RegisterTexture(default_texture, "default");
    }

    int Renderer::RegisterStaticMesh(const StaticMeshTraits &traits) {

      auto find = ssm.find(traits.shader_id);
      if (find == ssm.end()) {
        spdlog::error("Shader {} is invalid", traits.shader_id);
        return -1;
      }
      auto find2 = materials.find(traits.material_id);
      if (find2 == materials.end()) {
        spdlog::error("Material {} is invalid", traits.material_id);
        return -1;
      }
      ShadersStaticMeshes &ssm = find->second;

      spdlog::info("Registering static mesh {}", ssm.static_meshes.size());

      ssm.static_meshes.emplace_back(traits);

      return ssm.static_meshes.size() - 1;
    }
    std::optional<Renderer::DynamicMeshID> Renderer::RegisterDynamicMesh(const StaticMeshTraits &traits,
                                                                         const DynamicMeshID &id) {
      auto find = ssm.find(traits.shader_id);
      if (find == ssm.end()) {
        spdlog::error("Shader {} is invalid", traits.shader_id);
        return std::nullopt;
      }
      auto find2 = materials.find(traits.material_id);
      if (find2 == materials.end()) {
        spdlog::error("Material {} is invalid", traits.material_id);
        return std::nullopt;
      }
      spdlog::info("Registering dynamic mesh {}", id);

      dynamic_meshes.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                             std::forward_as_tuple(traits));
      return id;
    }
    void Renderer::DeregisterDynamicMesh(const DynamicMeshID &id) {

      spdlog::info("Deregistering dynamic mesh {}", id);
      dynamic_meshes.erase(id);
    }

    StaticMesh *Renderer::GetDynamicMesh(const DynamicMeshID &id) {
      if (auto mesh = dynamic_meshes.find(id); mesh != dynamic_meshes.end()) {
        return &mesh->second;
      } else {
        return nullptr;
      }
    }

    void Renderer::SetDynamicMeshShader(StaticMesh &mesh, const ShaderID &shader_id) {
      spdlog::info("Setting dynamic mesh shader to {}", shader_id);
      mesh.shader_id = shader_id;
    }

    std::optional<ShaderID> Renderer::RegisterShader(const ShaderTraits &traits) {

      Shader s(traits);
      if (s.GetCompileStatus() == 0) {
        spdlog::error("Shader failed to compile");
        return std::nullopt;
      }
      spdlog::info("Registering shader {}", traits.id);
      ssm.emplace(traits.id, std::move(s));

      return traits.id;
    }

    Shader *Renderer::GetShader(const ShaderID &id) {
      auto find = ssm.find(id);
      if (find == ssm.end()) {
        spdlog::error("Shader {} is invalid", id);
        return nullptr;
      }
      return &find->second.shader;
    }

    Material &Renderer::RegisterMaterial(const Material &material, const MaterialID &id) {
      return materials.emplace(id, material).first->second;
    }

    TextureID Renderer::RegisterTexture(const TextureTraits &traits, const TextureID &id) {
      return textures.emplace(id, traits).first->first;
    }
    void Renderer::Render() const {

      for (const auto &[shader_id, ssm] : ssm) {
        ssm.shader.Bind();

        for (const auto &mesh : ssm.static_meshes) {

          const Material &m = materials.at(mesh.material_id);
          textures.at(m.texture_id).Bind();
          ssm.shader.SetUniform3F("colour", m.albedo.r / 255.0f, m.albedo.g / 255.0f, m.albedo.b / 255.0f);
          ssm.shader.SetUniformMatrix3FV("v_model", 1, false, &mesh.transform.GetTransform()[0][0]);
          mesh.Draw();
        }
      }

      for (auto &[_, mesh] : dynamic_meshes) {

        const Shader &shader = ssm.at(mesh.shader_id).shader;
        shader.Bind();
        const Material &m = materials.at(mesh.material_id);
        textures.at(m.texture_id).Bind();
        shader.SetUniform3F("colour", m.albedo.r / 255.0f, m.albedo.g / 255.0f, m.albedo.b / 255.0f);

        shader.SetUniformMatrix3FV("v_model", 1, false, &mesh.transform.GetTransform()[0][0]);
        mesh.Draw();
      }
    }

  } // namespace Graphics
} // namespace Chemical
