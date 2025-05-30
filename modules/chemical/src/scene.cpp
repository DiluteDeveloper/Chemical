#include "chemical/scene.h"

namespace Chemical {

  Transform *Scene::CreateTransform(const ObjectID &id) { return &transforms[id]; }
  Transform *Scene::CreateTransform(const Transform &copy, const ObjectID &id) {
    return &transforms.emplace(id, copy).first->second;
  }
  Transform *Scene::GetTransform(const ObjectID &id) { return &transforms.at(id); }
  void Scene::DestroyTransform(const ObjectID &id) { transforms.erase(id); }

  // Graphics::Material *Scene::CreateMaterial(const ObjectID &id) { return &materials[id]; }
  // Graphics::Material *Scene::CreateMaterial(const Graphics::Material &copy, const ObjectID &id) {
  //   return &materials.emplace(id, copy).first->second;
  // }
  // Graphics::Material *Scene::GetMaterial(const ObjectID &id) { return &materials.at(id); }
  // void Scene::DestroyMaterial(const ObjectID &id) { materials.erase(id); }
  //
  // Graphics::Texture *Scene::CreateTexture(const Graphics::TextureTraits &traits, const ObjectID &id) {
  //   return &textures.emplace(id, traits).first->second;
  // }
  // Graphics::Texture *Scene::GetTexture(const ObjectID &id) { return &textures.at(id); }
  // void Scene::DestroyTexture(const ObjectID &id) { textures.erase(id); }

  // Graphics::StaticMesh *Scene::CreateDynamicMesh(const Graphics::DynamicMeshID &id) { return nullptr; }
  // Graphics::StaticMesh *Scene::GetDynamicMesh(const Graphics::DynamicMeshID &id) {}
  // void Scene::DestroyDynamicMesh(const Graphics::DynamicMeshID &id) {}

  void Scene::CreateStaticMesh(const Graphics::StaticMeshTraits &traits) {
    shader_mapped_static_meshes[traits.shader_id].emplace_back(traits);
  }
} // namespace Chemical
