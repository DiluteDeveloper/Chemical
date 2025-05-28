#pragma once

#include "chemical/util/image.h"

namespace Chemical {
  namespace Graphics {

    struct TextureTraits {
      enum class ScalingFilter { NEAREST = 0x2600, LINEAR = 0x2601 };
      Util::Image image;
      ScalingFilter scaling_filter = ScalingFilter::NEAREST;

      TextureTraits(const std::string_view &file_path) : image(Util::LoadImage(file_path)) {}
      TextureTraits(const Util::Image image_data) : image(image_data) {}
    };

    using TextureID = std::string;
    class Texture {
    public:
      Texture(const TextureTraits &traits);
      ~Texture();

      void Bind() const;
      Texture(Texture &&other) : gl_texture_id(other.gl_texture_id) { other.gl_texture_id = 0; }
      Texture &operator=(Texture &&other) {
        gl_texture_id = other.gl_texture_id;
        other.gl_texture_id = 0;
        return *this;
      }
      Texture(const Texture &other) = delete;
      Texture &operator=(const Texture &other) = delete;

    private:
      unsigned int gl_texture_id = 0;
    };
  } // namespace Graphics
} // namespace Chemical
