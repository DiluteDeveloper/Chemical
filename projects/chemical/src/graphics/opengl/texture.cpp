#include "pch.h"

#include <glad/glad.h>

#include "texture.h"

namespace OpenGL {
	/*uint16_t ConvertTextureSizedInternalFormatToBaseInternalFormat(TextureSizedInternalFormat sizedInternalFormat) {
		switch (sizedInternalFormat) {
		case TextureSizedInternalFormat::R8:
			return GL_RED;
		case TextureSizedInternalFormat::R8_SNORM:
			return GL_RED;
		case TextureSizedInternalFormat::R16:
			return GL_RED;
		case TextureSizedInternalFormat::R16_SNORM:
			return GL_RED;
		case TextureSizedInternalFormat::RG8:
			return GL_RG;
		case TextureSizedInternalFormat::RG8_SNORM:
			return GL_RG;
		case TextureSizedInternalFormat::RG16:
			return GL_RG;
		case TextureSizedInternalFormat::RG16_SNORM:
			return GL_RG;
		case TextureSizedInternalFormat::R3_G3_B2:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB4:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB5:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB8:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB8_SNORM:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB10:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB12:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB16_SNORM:
			return GL_RGB;
		case TextureSizedInternalFormat::RGBA2:
			return GL_RGB;
		case TextureSizedInternalFormat::RGBA4:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB5_A1:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA8:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA8_SNORM:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGB10_A2:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGB10_A2UI:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA12:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA16:
			return GL_RGBA;
		case TextureSizedInternalFormat::SRGB8:
			return GL_RG;
		case TextureSizedInternalFormat::SRGB8_ALPHA8:
			return GL_RGBA;
		case TextureSizedInternalFormat::R16F:
			return GL_RED;
		case TextureSizedInternalFormat::RG16F:
			return GL_RG;
		case TextureSizedInternalFormat::RGB16F:
			return GL_RGB;
		case TextureSizedInternalFormat::RGBA16F:
			return GL_RGBA;
		case TextureSizedInternalFormat::R32F:
			return GL_RED;
		case TextureSizedInternalFormat::RG32F:
			return GL_RG;
		case TextureSizedInternalFormat::RGB32F:
			return GL_RGB;
		case TextureSizedInternalFormat::RGBA32F:
			return GL_RGBA;
		case TextureSizedInternalFormat::R11F_G11F_B10F:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB9_E5:
			return GL_RGB;
		case TextureSizedInternalFormat::R8I:
			return GL_RED;
		case TextureSizedInternalFormat::R8UI:
			return GL_RED;
		case TextureSizedInternalFormat::R16I:
			return GL_RED;
		case TextureSizedInternalFormat::R16UI:
			return GL_RED;
		case TextureSizedInternalFormat::R32I:
			return GL_RED;
		case TextureSizedInternalFormat::R32UI:
			return GL_RED;
		case TextureSizedInternalFormat::RG8I:
			return GL_RG;
		case TextureSizedInternalFormat::RG8UI:
			return GL_RG;
		case TextureSizedInternalFormat::RG16I:
			return GL_RG;
		case TextureSizedInternalFormat::RG16UI:
			return GL_RG;
		case TextureSizedInternalFormat::RG32I:
			return GL_RG;
		case TextureSizedInternalFormat::RG32UI:
			return GL_RG;
		case TextureSizedInternalFormat::RGB8I:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB8UI:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB16I:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB16UI:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB32I:
			return GL_RGB;
		case TextureSizedInternalFormat::RGB32UI:
			return GL_RGB;
		case TextureSizedInternalFormat::RGBA8I:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA8UI:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA16I:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA16UI:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA32I:
			return GL_RGBA;
		case TextureSizedInternalFormat::RGBA32UI:
			return GL_RGBA;
		default:
			return GL_RGB;
		}
	}*/

	Texture::Texture(const TextureStorageParameters& parameters) {
		glCreateTextures((GLenum)parameters.textureType, 1, &m_rendererID);

		switch (parameters.textureType) {
		case TextureType::TEXTURE_1D:
			glTextureStorage1D(m_rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat, parameters.width);
			break;
		case TextureType::TEXTURE_1D_ARRAY:
			[[fallthrough]];
		case TextureType::TEXTURE_2D:
			glTextureStorage2D(m_rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat,
				parameters.width, parameters.height);
			break;
		case TextureType::TEXTURE_2D_ARRAY:
			[[fallthrough]];
		case TextureType::TEXTURE_3D:
			glTextureStorage3D(m_rendererID, parameters.mipLevels, (GLenum)parameters.internalFormat,
				parameters.width, parameters.height, parameters.depth);
			break;
		case TextureType::TEXTURE_2D_MULTISAMPLE:
			glTextureStorage2DMultisample(m_rendererID, parameters.samples, (GLenum)parameters.internalFormat,
				parameters.width, parameters.height, (GLboolean)parameters.fixedSampleLocations);
			break;
		case TextureType::TEXTURE_3D_MULTISAMPLE:
			glTextureStorage3DMultisample(m_rendererID, parameters.samples, (GLenum)parameters.internalFormat,
				parameters.width, parameters.height, parameters.depth, (GLboolean)parameters.fixedSampleLocations);
			break;
		}
	}

	// Sets a section/all of the buffers data
	void Texture::SetTextureData(const TextureDataParameters& parameters, void* data) {
		switch (parameters.textureType) {
		case TextureType::TEXTURE_1D:
			glTextureSubImage1D(m_rendererID, 0, parameters.widthOffset, 
				parameters.width, 
				(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
			break;
		case TextureType::TEXTURE_2D:
			[[fallthrough]];
		case TextureType::TEXTURE_1D_ARRAY:
			[[fallthrough]];
		case TextureType::TEXTURE_2D_MULTISAMPLE:
			glTextureSubImage2D(m_rendererID, 0, parameters.widthOffset, parameters.heightOffset,
				parameters.width, parameters.height,
				(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
			break;
		case TextureType::TEXTURE_3D:
			[[fallthrough]];
		case TextureType::TEXTURE_2D_ARRAY:
			[[fallthrough]];
		case TextureType::TEXTURE_3D_MULTISAMPLE:
			glTextureSubImage3D(m_rendererID, 0, parameters.widthOffset, parameters.heightOffset, parameters.depthOffset,
				parameters.width, parameters.height, parameters.depthOffset,
				(GLenum)parameters.baseFormat, (GLenum)parameters.dataType, data);
			break;
		}
	}

	void Texture::GenerateMipmaps() {
		glGenerateTextureMipmap(m_rendererID);
	}

	void Texture::SetTextureSetting(TextureSettings setting, int32_t value) {
		glTextureParameteri(m_rendererID, (GLenum)setting, (GLint)value);
	}
	void Texture::SetTextureSetting(TextureSettings setting, float* value) {
		glTextureParameterfv(m_rendererID, (GLenum)setting, (GLfloat*)value);
	}

	void Texture::BindTexture(uint32_t textureUnit) {
		glBindTextureUnit(textureUnit, m_rendererID);
	}
	Texture::~Texture() {
		glDeleteTextures(1, &m_rendererID);
	}
}