#pragma once

#include "chemical/util/image.h"

namespace Chemical {
  enum class TextureFilter { NEAREST = 0x2600, LINEAR = 0x2601 };

  struct TextureTraits {

    TextureFilter texture_filter = TextureFilter::NEAREST;
    Util::Image image;

    TextureTraits(const Util::Image& image) : image(image) {}
  };
} // namespace Chemical
