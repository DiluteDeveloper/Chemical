#pragma once

#include "chemical/graphics/static_mesh.h"

#include <unordered_map>
#include <vector>

namespace Chemical {
  namespace Graphics {

    // Dynamic meshes are represented by static meshes temporarily until
    // dynamic meshes are actually implemented
    class Renderer {
    public:
      // Will be moved to new DynamicMesh class in a future version
      using DynamicMeshID = int;

      Renderer();

      StaticMeshID RegisterStaticMesh(const StaticMeshTraits &traits);

      DynamicMeshID RegisterDynamicMesh(const StaticMeshTraits &traits);

      void DeregisterDynamicMesh(DynamicMeshID id);

      StaticMesh *GetStaticMesh(StaticMeshID id);
      StaticMesh *GetDynamicMesh(DynamicMeshID id);

      void SetDynamicMeshShader(StaticMesh &mesh, ShaderID shader_id);

      ShaderID RegisterShader(const ShaderTraits &traits);

      Shader *GetShader(ShaderID id);

      static constexpr ShaderID GetDefaultShaderID() { return 0; };

      void Render() const;

    private:
      std::vector<Shader> shaders;

      std::unordered_map<ShaderID, std::vector<StaticMesh *>> shader_mesh_map;

      std::vector<StaticMesh> static_meshes;

      std::unordered_map<DynamicMeshID, StaticMesh> dynamic_meshes;
    };
  } // namespace Graphics
} // namespace Chemical
