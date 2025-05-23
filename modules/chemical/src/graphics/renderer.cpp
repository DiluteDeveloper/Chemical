#include "chemical/graphics/renderer.h"

#include <spdlog/spdlog.h>
#include <tuple>
#include <utility>

namespace Chemical {
  namespace Graphics {

    Renderer::Renderer() {

      spdlog::info("Initialising and registering default 2D shader");
      // Create and register default 2D shader
      ShaderTraits s;
      s.vs_file_path = "shaders/test_shader.vs";
      s.fs_file_path = "shaders/test_shader.fs";

      RegisterShader(s);
    }

    StaticMeshID Renderer::RegisterStaticMesh(const StaticMeshTraits &traits) {
      if (traits.shader_id >= shaders.size()) {
        spdlog::error("Shader {} is invalid", traits.shader_id);
        return -1;
      }

      spdlog::info("Registering static mesh {}", static_meshes.size());

      static_meshes.emplace_back(traits);

      if (auto v = shader_mesh_map.find(traits.shader_id); v != shader_mesh_map.end()) {
        v->second.emplace_back(&static_meshes[static_meshes.size() - 1]);
      }
      return static_meshes.size() - 1;
    }
    Renderer::DynamicMeshID Renderer::RegisterDynamicMesh(const StaticMeshTraits &traits) {
      if (traits.shader_id >= shaders.size()) {
        spdlog::error("Shader {} is invalid", traits.shader_id);
        return -1;
      }

      spdlog::info("Registering dynamic mesh {}", dynamic_meshes.size());

      dynamic_meshes.emplace(std::piecewise_construct, std::forward_as_tuple(dynamic_meshes.size()),
                             std::forward_as_tuple(traits));
      return dynamic_meshes.size() - 1;
    }
    void Renderer::DeregisterDynamicMesh(DynamicMeshID id) {

      spdlog::info("Deregistering dynamic mesh {}", id);
      dynamic_meshes.erase(id);
    }

    StaticMesh *Renderer::GetStaticMesh(StaticMeshID id) {
      if (id < static_meshes.size())
        return &static_meshes[id];
      else
        return nullptr;
    }
    StaticMesh *Renderer::GetDynamicMesh(DynamicMeshID id) {
      if (auto mesh = dynamic_meshes.find(id); mesh != dynamic_meshes.end()) {
        return &mesh->second;
      } else
        return nullptr;
    }

    void Renderer::SetDynamicMeshShader(StaticMesh &mesh, ShaderID shader_id) {
      spdlog::info("Setting dynamic mesh shader to {}", shader_id);
      mesh.shader_id = shader_id;
    }

    ShaderID Renderer::RegisterShader(const ShaderTraits &traits) {

      Shader s(traits);
      if (s.GetCompileStatus() == 0) {
        spdlog::error("Shader failed to compile");
        return -1;
      }
      spdlog::info("Registering shader {}", shaders.size());
      shaders.emplace_back(std::move(s));
      shader_mesh_map.emplace(shaders.size() - 1, std::move(std::vector<StaticMesh *>()));

      assert(shader_mesh_map.size() == shaders.size());

      return shaders.size() - 1;
    }

    Shader *Renderer::GetShader(ShaderID id) {
      if (id < shaders.size())
        return &shaders[id];
      else
        return nullptr;
    }

    void Renderer::Render() const {

      unsigned int idx = 0;
      for (auto &shader : shaders) {
        shader.Bind();
        const std::vector<StaticMesh *> &meshes = shader_mesh_map.at(idx);
        for (auto mesh : meshes) {
          mesh->Draw();
        }
        idx++;
      }

      for (auto &[_, mesh] : dynamic_meshes) {
        shaders[mesh.shader_id].Bind();
        mesh.Draw();
      }
    }

  } // namespace Graphics
} // namespace Chemical
