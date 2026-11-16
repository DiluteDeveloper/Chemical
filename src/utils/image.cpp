#include "image.h"
#include <spdlog/spdlog.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Chemical {

    std::unique_ptr<Image> Image::LoadImage(const char* image_path, int desired_channels) {

        int x,y,n;
        unsigned char* data = stbi_load(image_path, &x, &y, &n, desired_channels);
        if(!data)
            return nullptr;
        if(desired_channels != n)
            spdlog::warn("Number of channels requested does not match number of channels received!");
        return std::make_unique<Image>(Image{(uint32_t)x, (uint32_t)y, (uint32_t)n, data});
    }
    Image::Image(uint32_t x, uint32_t y, uint32_t num_channels, const unsigned char* data) :
        x(x), y(y), num_channels(num_channels), data(data) {}
    Image::~Image() {
        stbi_image_free((void*)data);
    }
}
