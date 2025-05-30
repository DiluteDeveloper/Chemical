#pragma once

#include "chemical/graphics/material.h"
#include "chemical/graphics/static_mesh.h"
#include "chemical/graphics/texture.h"
#include "util/transform.h"

#include <unordered_map>

namespace Chemical {
  namespace Graphics {
    class Renderer;
  }

  using ObjectID = std::string;

  class Scene {
  public:
    Transform *CreateTransform(const ObjectID &id);
    Transform *CreateTransform(const Transform &copy, const ObjectID &id);
    Transform *GetTransform(const ObjectID &id);
    void DestroyTransform(const ObjectID &id);

    // Dynamic meshes are currently not implemented
    // Graphics::StaticMesh *CreateDynamicMesh(const Graphics::DynamicMeshID &id);
    // Graphics::StaticMesh *GetDynamicMesh(const Graphics::DynamicMeshID &id);
    // void DestroyDynamicMesh(const Graphics::DynamicMeshID &id);

    void CreateStaticMesh(const Graphics::StaticMeshTraits &traits);

    Scene() = default;

  protected:
    friend class Graphics::Renderer;

    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;

    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    std::unordered_map<ObjectID, Transform> transforms;

    // std::unordered_map<Graphics::DynamicMeshID, Graphics::StaticMesh> dynamic_meshes;
    std::unordered_map<ObjectID, std::vector<Graphics::StaticMesh>> shader_mapped_static_meshes;
  };
} // namespace Chemical
