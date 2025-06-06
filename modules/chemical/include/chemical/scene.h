#pragma once

#include "chemical/camera.h"
#include "chemical/graphics/material.h"
#include "chemical/graphics/shader.h"
#include "chemical/graphics/static_sprite.h"
#include "chemical/graphics/texture.h"
#include "chemical/scene_script.h"
#include "util/transform.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Chemical {
  namespace Graphics { class Renderer; }

  using ObjectID = std::string;
  class Core;

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

    template <class... CameraArgs>
    std::pair<Camera &, bool> CreateCamera(const ObjectID &id, CameraArgs &&...camera_args) {
      auto it = cameras.try_emplace(id, camera_args...);
      return std::pair<Camera &, bool>(it.first->second, it.second);
    }
    Camera *GetCamera(const ObjectID &id);
    void SetPrimaryCamera(const ObjectID &id);

    bool MoveConstructShader(const ObjectID &id, Graphics::Shader &&mv);

    template <class... StaticSpriteArgs>
    void CreateStaticSprite(const ObjectID &shader_id, StaticSpriteArgs... static_sprite_args) {
      static_sprites[shader_id].emplace_back(static_sprite_args...);
    }

    template <class SceneScriptType> void MountSceneScript(SafePtr<Core> core) {
      scripts.emplace_back(std::make_unique<SceneScriptType>(core))->BeginScript();
    }

    Scene(const std::string_view &file_path);
    ~Scene();

    std::string GetResourcePath(const std::string_view &affix);

    Scene &operator=(Scene &&) = default;
    Scene(Scene &&) = default;
    Scene(const Scene &) = delete;
    Scene &operator=(const Scene &) = delete;

  protected:
    friend class Graphics::Renderer;
    friend class Core;

    void UpdateScene();

    std::unordered_map<ObjectID, Transform> transforms;
    std::unordered_map<ObjectID, Graphics::Material> materials;
    std::unordered_map<ObjectID, Graphics::Texture> textures;
    std::unordered_map<ObjectID, Graphics::Shader> shaders;

    std::unordered_map<ObjectID, Camera> cameras;
    ObjectID primary_camera = "default";

    std::unordered_map<ObjectID, std::vector<Graphics::StaticSprite>> static_sprites;

    std::vector<std::unique_ptr<SceneScript>> scripts;

    std::string file_path = "";
  };
} // namespace Chemical
