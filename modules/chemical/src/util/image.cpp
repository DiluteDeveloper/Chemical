#include "chemical/util/image.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image/stb_image.h>
#include <string_view>
namespace Chemical {

  namespace Util {

    Image LoadImage(const std::string_view &file_path, int req_comp) {
      Image id;
      id.data = stbi_load(file_path.data(), &id.width, &id.height, &id.bit_depth, req_comp);
      return id;
    }
  } // namespace Util
} // namespace Chemical
