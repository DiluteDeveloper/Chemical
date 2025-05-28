#include "chemical/graphics/texture.h"

#include <glad/glad.h>

namespace Chemical {
  namespace Graphics {

    Texture::Texture(const TextureTraits &traits) {
      glCreateTextures(GL_TEXTURE_2D, 1, &gl_texture_id);

      glTextureParameteri(gl_texture_id, GL_TEXTURE_MIN_FILTER, (GLint)traits.scaling_filter);
      glTextureParameteri(gl_texture_id, GL_TEXTURE_MAG_FILTER, (GLint)traits.scaling_filter);

      glTextureStorage2D(gl_texture_id, 1, GL_RGBA8, traits.image.width, traits.image.height);
      glTextureSubImage2D(gl_texture_id, 0, 0, 0, traits.image.width, traits.image.height, GL_RGBA,
                          GL_UNSIGNED_BYTE, &traits.image.data[0]);

      glGenerateTextureMipmap(gl_texture_id);
    }
    Texture::~Texture() { glDeleteTextures(1, &gl_texture_id); }

    void Texture::Bind() const { glBindTextureUnit(0, gl_texture_id); }
  } // namespace Graphics
} // namespace Chemical
