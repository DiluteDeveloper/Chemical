#include "chemical/graphics/opengl/gl_material.h"

#include <glad/glad.h>

namespace Chemical {

  void GL_Material::CreateGLMaterial(const MaterialTraits& traits) {

    texture_filter = traits.texture_filter;
    tint = traits.tint;
    shader_id = traits.shader_id;

    glCreateTextures(GL_TEXTURE_2D, 1, &texture_id);

    glTextureParameteri(texture_id, GL_TEXTURE_MIN_FILTER, (GLint)texture_filter);
    glTextureParameteri(texture_id, GL_TEXTURE_MAG_FILTER, (GLint)texture_filter);

    glTextureStorage2D(texture_id, 1, GL_RGBA8, traits.image.width, traits.image.height);
    glTextureSubImage2D(texture_id, 0, 0, 0, traits.image.width, traits.image.height, GL_RGBA,
                        GL_UNSIGNED_BYTE, &traits.image.data[0]);

    glGenerateTextureMipmap(texture_id);
  }
  void GL_Material::DeleteGLMaterial() {
    glDeleteTextures(1, &texture_id);
  }
} // namespace Chemical
