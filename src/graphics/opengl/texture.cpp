#include <GLES3/gl3.h>

#include "texture.h"

namespace Chemical {


	namespace OpenGL {

		Texture::Texture(const TextureStorageParameters& parameters) :
			type(parameters.textureType) {
			glGenTextures(1, &rendererID);
			glBindTexture((GLenum)type, rendererID); // creates the texture object

			switch (type) {
			case TextureType::TEXTURE_2D:
				glTexStorage2D(GL_TEXTURE_2D, parameters.mipLevels, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height);
				break;
			case TextureType::TEXTURE_2D_ARRAY:
				glTexStorage3D(GL_TEXTURE_2D_ARRAY, parameters.mipLevels, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height, parameters.depth);
				break;
			case TextureType::TEXTURE_3D:
				glTexStorage3D(GL_TEXTURE_3D, parameters.mipLevels, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height, parameters.depth);
				break;
			}
		}

		// Sets a section/all of the texture's data
		void Texture::SetTextureData(const TextureDataParameters& parameters, const void* data) {
			glBindTexture((GLenum)type, rendererID);
			// Tightly packed rows (e.g. RGB8 with an odd width) break with the default alignment of 4.
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			switch (type) {
			case TextureType::TEXTURE_2D:
				glTexSubImage2D(GL_TEXTURE_2D, parameters.mipLevel, parameters.widthOffset, parameters.heightOffset,
					parameters.width, parameters.height,
					(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
				break;
			case TextureType::TEXTURE_3D:
			case TextureType::TEXTURE_2D_ARRAY:
				glTexSubImage3D((GLenum)type, parameters.mipLevel, parameters.widthOffset, parameters.heightOffset, parameters.depthOffset,
					parameters.width, parameters.height, parameters.depth,
					(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
				break;
			}
		}

		void Texture::GenerateMipmaps() {
			glBindTexture((GLenum)type, rendererID);
			glGenerateMipmap((GLenum)type);
		}

		void Texture::SetTextureSetting(TextureSettings setting, int32_t value) {
			glBindTexture((GLenum)type, rendererID);
			glTexParameteri((GLenum)type, (GLenum)setting, (GLint)value);
		}
		void Texture::SetTextureSetting(TextureSettings setting, const float* value) {
			glBindTexture((GLenum)type, rendererID);
			glTexParameterfv((GLenum)type, (GLenum)setting, (const GLfloat*)value);
		}

		void Texture::BindTexture(uint32_t textureUnit) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture((GLenum)type, rendererID);
		}

		void Texture::Release() {
			if (rendererID != 0) {
				glDeleteTextures(1, &rendererID);
				rendererID = 0;
			}
		}
		Texture::~Texture() {
			Release();
		}
	}
}
