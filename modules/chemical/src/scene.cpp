#include "chemical/scene.h"

#include <spdlog/spdlog.h>

namespace Chemical {
  bool Scene::RegisterSprite(const std::string_view& id, const Sprite& sprite) {
    return sprites.try_emplace(id.data(), sprite).second;
  }
  void Scene::DeregisterSprite(const std::string_view& id) {
    sprites.erase(id.data());
  }
  bool Scene::IsSprite(const std::string_view& id) {
    return sprites.contains(id.data());
  }
  Sprite* Scene::GetSprite(const std::string_view& id) {
    auto find = sprites.find(id.data());
    if (find == sprites.end()) {
      SPDLOG_WARN(R"(Tried to get sprite "{}" that does not exist)", id);
      return nullptr;
    }
    return &find->second;
  }
  bool Scene::RegisterTransform(const std::string_view& id, const Transform& transform) {
    return transforms.try_emplace(id.data(), transform).second;
  }
  void Scene::DeregisterTransform(const std::string_view& id) {
    transforms.erase(id.data());
  }
  bool Scene::IsTransform(const std::string_view& id) {
    return transforms.contains(id.data());
  }
  Transform* Scene::GetTransform(const std::string_view& id) {
    auto find = transforms.find(id.data());
    if (find == transforms.end()) {
      SPDLOG_WARN(R"(Tried to get transform "{}" that does not exist)", id);
      return nullptr;
    }
    return &find->second;
  }
  bool Scene::RegisterMaterial(const std::string_view& id, const Material& material) {
    return materials.try_emplace(id.data(), material).second;
  }
  void Scene::DeregisterMaterial(const std::string_view& id) {
    materials.erase(id.data());
  }
  bool Scene::IsMaterial(const std::string_view& id) {
    return materials.contains(id.data());
  }
  Material* Scene::GetMaterial(const std::string_view& id) {
    auto find = materials.find(id.data());
    if (find == materials.end()) {
      SPDLOG_WARN(R"(Tried to get material "{}" that does not exist)", id);
      return nullptr;
    }
    return &find->second;
  }

} // namespace Chemical
