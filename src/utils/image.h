#pragma once

#include <spdlog/spdlog.h>
#include <cstdint>

namespace Chemical {

    class Image {
    public:
        [[nodiscard]] static std::unique_ptr<Image> LoadImage(const char* image_path, int desired_channels = 4);
        Image(const Image&) = delete;
        Image(Image&& other) :
        x(other.x), y(other.y), num_channels(other.num_channels), data(other.data){
            spdlog::info("MOVE");
            other.data = nullptr;
        }
        Image& operator=(const Image&) = delete;
        Image& operator=(Image&& other) {
            spdlog::info("MOVE");
            other.data = nullptr;
            return *this;
        }

        const uint32_t x;
        const uint32_t y;
        const uint32_t num_channels;
        const unsigned char* data;

        ~Image();
    private:
        Image(uint32_t x, uint32_t y, uint32_t num_channels, const unsigned char* data);

    };
}
