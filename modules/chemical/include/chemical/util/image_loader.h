#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image/stb_image.h>
#include <string_view>
namespace Chemical {
  namespace Util {
    struct Image {
      int x = 0;
      int y = 0;
      int bitDepth = 0;
      unsigned char *data = nullptr;
      Image(int x, int y, int bitDepth, unsigned char *data) : x(x), y(y), bitDepth(bitDepth), data(data) {}
      Image() {}
    };

    // Always outputs a bit depth of 8
    inline Image LoadImage(const std::string_view &filePath, int req_comp = 0) {
      Image id;
      id.data = stbi_load(filePath.data(), &id.x, &id.y, &id.bitDepth, req_comp);
      return id;
    }

  } // namespace Util

} // namespace Chemical
