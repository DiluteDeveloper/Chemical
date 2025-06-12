#pragma once

#include "io/image.h"

#include <memory>

namespace Chemical {

  enum class TextureFilter { NEAREST = 0x2600, LINEAR = 0x2601 };

  struct TextureTraits {

    TextureFilter texture_filter = TextureFilter::NEAREST;
    std::shared_ptr<Image> image = nullptr;

    TextureTraits(std::shared_ptr<Image> image = nullptr,
                  TextureFilter texture_filter = TextureFilter::NEAREST)
        : image(image), texture_filter(texture_filter) {}
  };
} // namespace Chemical
