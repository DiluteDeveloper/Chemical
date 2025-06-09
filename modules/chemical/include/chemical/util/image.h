#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace Chemical {
  namespace Util {
    struct Image {
      int width = 0;
      int height = 0;
      unsigned char* data = nullptr;
      Image(int width, int height, unsigned char* data) : width(width), height(height), data(data) {}
      Image() {}
    };
    // Always outputs a bit depth of 8
    extern std::optional<Image> LoadImage(const std::string_view& file_path);

  } // namespace Util

} // namespace Chemical
