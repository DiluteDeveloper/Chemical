#include "chemical/util/image.h"

#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image/stb_image.h>
#include <string_view>
namespace Chemical {

  namespace Util {

    std::optional<Image> LoadImage(const std::string_view &file_path, int req_comp) {

      SPDLOG_INFO(R"(Loading image "{}")", file_path);
      Image id;
      id.data = stbi_load(file_path.data(), &id.width, &id.height, &id.bit_depth, req_comp);
      if (id.data == nullptr) {
        SPDLOG_ERROR(R"(Failed to load image "{}" : possible invalid filepath? returning std::nullopt)",
                     file_path);
        return std::nullopt;
      }
      stbi_image_free(id.data);

      return id;
    }
  } // namespace Util
} // namespace Chemical
