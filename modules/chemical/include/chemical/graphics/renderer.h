#pragma once

#include "chemical/graphics/static_material.h"
#include "chemical/graphics/static_mesh.h"

#include <optional>
#include <unordered_map>
#include <vector>

namespace Chemical {
  namespace Graphics {

    struct ShadersStaticMeshes;
    // Dynamic meshes are represented by static meshes temporarily until
    // dynamic meshes are actually implemented
    class Renderer {
    public:
      // Will be moved to new DynamicMesh class in a future version
      using DynamicMeshID = std::string;

      Renderer();

      int RegisterStaticMesh(const StaticMeshTraits &traits);

      std::optional<DynamicMeshID> RegisterDynamicMesh(const StaticMeshTraits &traits,
                                                       const DynamicMeshID &id);
      void DeregisterDynamicMesh(const DynamicMeshID &id);
      StaticMesh *GetDynamicMesh(const DynamicMeshID &id);
      void SetDynamicMeshShader(StaticMesh &mesh, const ShaderID &shader_id);

      std::optional<ShaderID> RegisterShader(const ShaderTraits &traits);
      Shader *GetShader(const ShaderID &id);

      Material &RegisterMaterial(const Material &material, const MaterialID &id);

      void Render() const;

    private:
      std::unordered_map<ShaderID, ShadersStaticMeshes> ssm;

      std::unordered_map<DynamicMeshID, StaticMesh> dynamic_meshes;

      std::unordered_map<MaterialID, Material> materials;
    };

    struct ShadersStaticMeshes {
      Shader shader;
      std::vector<StaticMesh> static_meshes;

      ShadersStaticMeshes(const ShaderTraits &traits) : shader(traits) {}
      ShadersStaticMeshes(Shader &&mv_shader) : shader(std::move(mv_shader)) {}
    };
  } // namespace Graphics
} // namespace Chemical
