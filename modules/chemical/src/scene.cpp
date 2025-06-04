#include "chemical/scene.h"

#include <spdlog/spdlog.h>

namespace Chemical {

  Transform *Scene::GetTransform(const ObjectID &id) {
    auto find = transforms.find(id);
    if (find == transforms.end()) {
      SPDLOG_ERROR(R"(Tried to get transform "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }
  void Scene::DestroyTransform(const ObjectID &id) {
    transforms.erase(id);
  }

  Graphics::Material *Scene::GetMaterial(const ObjectID &id) {
    auto find = materials.find(id);
    if (find == materials.end()) {
      SPDLOG_ERROR(R"(Tried to get material "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }

  Graphics::Texture *Scene::GetTexture(const ObjectID &id) {
    auto find = textures.find(id);
    if (find == textures.end()) {
      SPDLOG_ERROR(R"(Tried to get texture "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }
  Camera *Scene::GetCamera(const ObjectID &id) {

    auto find = cameras.find(id);
    if (find == cameras.end()) {
      SPDLOG_ERROR(R"(Tried to get camera "{}" that does not exist!)", id);
      return nullptr;
    }
    return &find->second;
  }
  void Scene::SetPrimaryCamera(const ObjectID &id) {
    primary_camera = id;
  }

  bool Scene::MoveConstructShader(const ObjectID &id, Graphics::Shader &&mv) {
    if (mv.compile_status == -1) {
      SPDLOG_ERROR(R"(Failed to move construct shader "{}" : shader doesnt compile!)", id);
      return false;
    }
    shaders.try_emplace(id, std::move(mv));
    return true;
  }
  void Scene::UpdateScene() {
    for (auto &script : scripts) {
      script->FrameUpdate();
    }
  }

  Scene::Scene(const std::string_view &file_path) : file_path(file_path) {

    Graphics::ShaderTraits traits("default");

    traits.vs_file_path = GetResourcePath("shaders/test_shader.vs");
    traits.fs_file_path = GetResourcePath("shaders/test_shader.fs");

    Graphics::Shader default_shader(traits);

    MoveConstructShader("default", std::move(default_shader));

    CreateTransform("default");
    CreateTransform("camera");

    CreateCamera("default", "camera");
  }
  Scene::~Scene() {
    for (auto &script : scripts) {
      script->EndScript();
    }
  }

  std::string Scene::GetResourcePath(const std::string_view &affix) {
    return std::string(file_path + affix.data());
  }

} // namespace Chemical
