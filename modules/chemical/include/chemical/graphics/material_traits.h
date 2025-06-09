#pragma once

#include "chemical/util/image.h"

#include <glm/glm.hpp>
#include <string>

namespace Chemical {
  enum class TextureFilter { NEAREST = 0x2600, LINEAR = 0x2601 };

  // Inert and not specific to a rendering library; to be processed by a rendering library
  struct MaterialTraits {
    TextureFilter texture_filter;
    glm::uvec3 tint = glm::uvec3(255);
    std::string shader_id = "default";
    const Util::Image image;

    MaterialTraits(const Util::Image& image) : image(image) {}
  };

} // namespace Chemical
