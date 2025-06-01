#include "chemical/graphics/texture.h"

#include "chemical/util/image.h"
#include "spdlog/spdlog.h"

#include <glad/glad.h>

namespace Chemical {
  namespace Graphics {

    Texture::Texture(const Util::Image &image, ScalingFilter scaling_filter) {

      glCreateTextures(GL_TEXTURE_2D, 1, &gl_id);
      SPDLOG_INFO(R"(Initialising OpenGL texture {})", gl_id);

      glTextureParameteri(gl_id, GL_TEXTURE_MIN_FILTER, (GLint)scaling_filter);
      glTextureParameteri(gl_id, GL_TEXTURE_MAG_FILTER, (GLint)scaling_filter);

      glTextureStorage2D(gl_id, 1, GL_RGB8, image.width, image.height);
      glTextureSubImage2D(gl_id, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE,
                          &image.data[0]);

      glGenerateTextureMipmap(gl_id);
    }
    Texture::~Texture() {
      SPDLOG_INFO(R"(Deleting OpenGL texture {})", gl_id);
      glDeleteTextures(1, &gl_id);
    }

    void Texture::Bind() const {
      glBindTextureUnit(0, gl_id);
    }
  } // namespace Graphics
} // namespace Chemical
