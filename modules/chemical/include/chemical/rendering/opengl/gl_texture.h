#pragma once

#include "rendering/texture.h"

#include <glad/glad.h>

namespace Chemical {

  namespace OpenGL {

    inline uint32_t CreateTexture(const TextureTraits& traits) {

      uint32_t texture = 0;
      glCreateTextures(GL_TEXTURE_2D, 1, &texture);

      glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, (GLint)traits.texture_filter);
      glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, (GLint)traits.texture_filter);

      glTextureStorage2D(texture, 1, GL_RGBA8, traits.image->width, traits.image->height);
      glTextureSubImage2D(texture, 0, 0, 0, traits.image->width, traits.image->height, GL_RGBA,
                          GL_UNSIGNED_BYTE, &traits.image->data[0]);

      glGenerateTextureMipmap(texture);

      return texture;
    };
  } // namespace OpenGL

} // namespace Chemical
