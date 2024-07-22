#include "pch.h"

#include <glad/glad.h>

#include "texture.h"


namespace Chemical {


	namespace OpenGL {

		Texture::Texture(const TextureStorageParameters& parameters) {
			glCreateTextures((GLenum)parameters.textureType, 1, &rendererID);

			switch (parameters.textureType) {
			case TextureType::TEXTURE_1D:
				glTextureStorage1D(rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat, parameters.width);
				break;
			case TextureType::TEXTURE_1D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_2D:
				glTextureStorage2D(rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height);
				break;
			case TextureType::TEXTURE_2D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_3D:
				glTextureStorage3D(rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height, parameters.depth);
				break;
			case TextureType::TEXTURE_2D_MULTISAMPLE:
				glTextureStorage2DMultisample(rendererID, parameters.samples, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height, (GLboolean)parameters.fixedSampleLocations);
				break;
			case TextureType::TEXTURE_3D_MULTISAMPLE:
				glTextureStorage3DMultisample(rendererID, parameters.samples, (GLenum)parameters.internalFormat,
					parameters.width, parameters.height, parameters.depth, (GLboolean)parameters.fixedSampleLocations);
				break;
			}
		}

		// Sets a section/all of the buffers data
		void Texture::SetTextureData(const TextureDataParameters& parameters, void* data) {
			switch (parameters.textureType) {
			case TextureType::TEXTURE_1D:
				glTextureSubImage1D(rendererID, 0, parameters.widthOffset,
					parameters.width,
					(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
				break;
			case TextureType::TEXTURE_2D:
				[[fallthrough]];
			case TextureType::TEXTURE_1D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_2D_MULTISAMPLE:
				glTextureSubImage2D(rendererID, 0, parameters.widthOffset, parameters.heightOffset,
					parameters.width, parameters.height,
					(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
				break;
			case TextureType::TEXTURE_3D:
				[[fallthrough]];
			case TextureType::TEXTURE_2D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_3D_MULTISAMPLE:
				glTextureSubImage3D(rendererID, 0, parameters.widthOffset, parameters.heightOffset, parameters.depthOffset,
					parameters.width, parameters.height, parameters.depthOffset,
					(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
				break;
			}
		}

		void Texture::GenerateMipmaps() {
			glGenerateTextureMipmap(rendererID);
		}

		void Texture::SetTextureSetting(TextureSettings setting, int32_t value) {
			glTextureParameteri(rendererID, (GLenum)setting, (GLint)value);
		}
		void Texture::SetTextureSetting(TextureSettings setting, float* value) {
			glTextureParameterfv(rendererID, (GLenum)setting, (GLfloat*)value);
		}

		void Texture::BindTexture(uint32_t textureUnit) {
			glBindTextureUnit(textureUnit, rendererID);
		}
		Texture::~Texture() {
			glDeleteTextures(1, &rendererID);
		}
	}
}
