#pragma once

#include "chemical/graphics/material.h"
#include "chemical/graphics/shader.h"
#include "chemical/graphics/static_mesh.h"
#include "chemical/graphics/texture.h"
#include "util/transform.h"

#include <iostream>
#include <unordered_map>

namespace Chemical {
  namespace Graphics { class Renderer; }

  using ObjectID = std::string;

  class Scene {
  public:
    template <class... TransformArgs>
    std::pair<Transform &, bool> CreateTransform(const ObjectID &id, TransformArgs &&...transform_args) {
      auto it = transforms.try_emplace(id, transform_args...);
      return std::pair<Transform &, bool>(it.first->second, it.second);
    }
    Transform *GetTransform(const ObjectID &id);
    void DestroyTransform(const ObjectID &id);

    template <class... MaterialArgs>
    std::pair<Graphics::Material &, bool> CreateMaterial(const ObjectID &id,
                                                         MaterialArgs &&...material_args) {
      auto it = materials.try_emplace(id, material_args...);
      return std::pair<Graphics::Material &, bool>(it.first->second, it.second);
    }
    Graphics::Material *GetMaterial(const ObjectID &id);

    template <class... TextureArgs>
    std::pair<Graphics::Texture &, bool> CreateTexture(const ObjectID &id, TextureArgs &&...texture_args) {
      auto it = textures.try_emplace(id, texture_args...);
      return std::pair<Graphics::Texture &, bool>(it.first->second, it.second);
    }
    Graphics::Texture *GetTexture(const ObjectID &id);

    bool MoveConstructShader(const ObjectID &id, Graphics::Shader &&mv);

    template <class... StaticMeshArgs>
    void CreateStaticMesh(const ObjectID &shader_id, StaticMeshArgs... static_mesh_args) {
      shader_mapped_static_meshes[shader_id].emplace_back(static_mesh_args...);
    }

    Scene(const std::string_view &file_path);

    std::string GetResourcePath(const std::string_view &affix);

  protected:
    friend class Graphics::Renderer;

    Scene(const Scene &) = delete;
    Scene(Scene &&) = delete;

    Scene &operator=(const Scene &) = delete;
    Scene &operator=(Scene &&) = delete;

    std::unordered_map<ObjectID, Transform> transforms;
    std::unordered_map<ObjectID, Graphics::Material> materials;
    std::unordered_map<ObjectID, Graphics::Texture> textures;
    std::unordered_map<ObjectID, Graphics::Shader> shaders;

    std::unordered_map<ObjectID, std::vector<Graphics::StaticMesh>> shader_mapped_static_meshes;

    std::string file_path = "";
  };
} // namespace Chemical
