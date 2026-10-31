#pragma once

#include "rendering/texture.h"

#include <GLES3/gl3.h>

#include <algorithm>
#include <cstdint>

namespace Chemical {

  namespace OpenGL {

    // OpenGL ES 3.0 / WebGL2 has no direct state access (glCreateTextures,
    // glTextureParameteri, ...), so the texture is bound to edit it and
    // unbound again afterwards.
    inline uint32_t CreateTexture(const TextureTraits& traits) {

      const GLsizei width = (GLsizei)traits.image->width;
      const GLsizei height = (GLsizei)traits.image->height;

      // Full mip chain: floor(log2(max(width, height))) + 1 levels.
      GLsizei levels = 1;
      for (GLsizei size = std::max(width, height); size > 1; size >>= 1) {
        levels++;
      }

      GLuint texture = 0;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)traits.texture_filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)traits.texture_filter);

      glTexStorage2D(GL_TEXTURE_2D, levels, GL_RGBA8, width, height);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE,
                      &traits.image->data[0]);

      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);

      return texture;
    }
  } // namespace OpenGL

} // namespace Chemical
