#include "io/image.h"

#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Chemical {
  Image::~Image() {
    free(data);
  }

  std::shared_ptr<Image> LoadImageFile(const std::string_view& file_path) {

    SPDLOG_INFO(R"(Loading image file "{}")", file_path);

    auto image = std::make_shared<Image>();
    image->data = stbi_load(file_path.data(), &image->width, &image->height, nullptr, 4);
    if (!image->data) {
      SPDLOG_ERROR(R"(Failed to load image "{}")", file_path);
      return nullptr;
    }
    return image;
  }
} // namespace Chemical
