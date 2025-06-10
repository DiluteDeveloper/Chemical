#pragma once

#include "chemical/graphics/material.h"
#include "chemical/graphics/sprite.h"
#include "chemical/util/transform.h"

#include <unordered_map>

namespace Chemical {

  // ISceneObjectRegistrationHandler
  struct I_SORHandler {
    virtual bool RegisterSprite(const std::string_view& id, const Sprite& sprite) = 0;
    virtual bool RegisterTransform(const std::string_view& id, const Transform& transform) = 0;
    virtual bool RegisterMaterial(const std::string_view& id, const Material& transform) = 0;
  };

  class Scene : public I_SORHandler {
  public:
    Scene() = default;

    bool RegisterSprite(const std::string_view& id, const Sprite& sprite);
    void DeregisterSprite(const std::string_view& id);
    bool IsSprite(const std::string_view& id);
    Sprite* GetSprite(const std::string_view& id);

    bool RegisterTransform(const std::string_view& id, const Transform& transform);
    void DeregisterTransform(const std::string_view& id);
    bool IsTransform(const std::string_view& id);
    Transform* GetTransform(const std::string_view& id);

    bool RegisterMaterial(const std::string_view& id, const Material& material);
    void DeregisterMaterial(const std::string_view& id);
    bool IsMaterial(const std::string_view& id);
    Material* GetMaterial(const std::string_view& id);

  protected:
    friend class GL_Renderer;

    std::unordered_map<std::string, Sprite> sprites;
    std::unordered_map<std::string, Transform> transforms;
    std::unordered_map<std::string, Material> materials;
  };
} // namespace Chemical
