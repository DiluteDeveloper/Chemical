#pragma once

#include <optional>
#include <string_view>

namespace Chemical {
  namespace Util {
    struct Image {
      int width = 0;
      int height = 0;
      int bit_depth = 0;
      unsigned char *data = nullptr;
      Image(int x, int y, int bit_depth, unsigned char *data)
          : width(x), height(y), bit_depth(bit_depth), data(data) {}
      Image() {}
    };
    // Always outputs a bit depth of 8
    extern std::optional<Image> LoadImage(const std::string_view &file_path, int req_comp = 0);

  } // namespace Util

} // namespace Chemical
