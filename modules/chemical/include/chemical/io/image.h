#pragma once

#include <memory>
#include <optional>
#include <string_view>

namespace Chemical {
  // data field is freed on destruction
  // C-like API because of STB_Image
  struct Image {
    int width = 0;
    int height = 0;
    unsigned char* data = nullptr;
    Image(int width = 0, int height = 0, unsigned char* data = nullptr)
        : width(width), height(height), data(data) {}

    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

    ~Image();
  };
  extern std::shared_ptr<Image> LoadImageFile(const std::string_view& file_path);

} // namespace Chemical
