#pragma once

#include "chemical/graphics/sprite.h"
#include "chemical/util/transform.h"

#include <optional>
#include <unordered_map>

namespace Chemical {

  class Scene {
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

  protected:
    friend class GL_Renderer;

    std::unordered_map<std::string, Sprite> sprites;
    std::unordered_map<std::string, Transform> transforms;
  };
} // namespace Chemical
