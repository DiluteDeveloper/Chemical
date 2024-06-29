#include "pch.h"

#include <glad/glad.h>

#include "texture.h"


namespace Chemical {


	namespace OpenGL {


		TextureBaseFormat ConvertInternalToBase(TextureInternalFormat internal_format) {
			switch (internal_format) {
			case TextureInternalFormat::R8:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R8_SNORM:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R16:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R16_SNORM:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::RG8:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG8_SNORM:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG16:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG16_SNORM:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::R3_G3_B2:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB4:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB5:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB8:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB8_SNORM:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB10:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB12:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB16_SNORM:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGBA2:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGBA4:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB5_A1:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA8:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA8_SNORM:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGB10_A2:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGB10_A2UI:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA12:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA16:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::SRGB8:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::SRGB8_ALPHA8:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::R16F:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::RG16F:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RGB16F:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGBA16F:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::R32F:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::RG32F:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RGB32F:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGBA32F:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::R11F_G11F_B10F:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB9_E5:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::R8I:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R8UI:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R16I:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R16UI:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R32I:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::R32UI:
				return OpenGL::TextureBaseFormat::RED;
			case TextureInternalFormat::RG8I:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG8UI:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG16I:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG16UI:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG32I:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RG32UI:
				return OpenGL::TextureBaseFormat::RG;
			case TextureInternalFormat::RGB8I:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB8UI:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB16I:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB16UI:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB32I:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGB32UI:
				return OpenGL::TextureBaseFormat::RGB;
			case TextureInternalFormat::RGBA8I:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA8UI:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA16I:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA16UI:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA32I:
				return OpenGL::TextureBaseFormat::RGBA;
			case TextureInternalFormat::RGBA32UI:
				return OpenGL::TextureBaseFormat::RGBA;
			default:
				return OpenGL::TextureBaseFormat::RGB;
			}
		}

		Texture::Texture(const TextureStorageParameters& parameters) {
			glCreateTextures((GLenum)parameters.texture_type, 1, &renderer_id);

			switch (parameters.texture_type) {
			case TextureType::TEXTURE_1D:
				glTextureStorage1D(renderer_id, parameters.mip_levels, (GLenum)parameters.internal_format, parameters.width);
				break;
			case TextureType::TEXTURE_1D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_2D:
				glTextureStorage2D(renderer_id, parameters.mip_levels, (GLenum)parameters.internal_format,
					parameters.width, parameters.height);
				break;
			case TextureType::TEXTURE_2D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_3D:
				glTextureStorage3D(renderer_id, parameters.mip_levels, (GLenum)parameters.internal_format,
					parameters.width, parameters.height, parameters.depth);
				break;
			case TextureType::TEXTURE_2D_MULTISAMPLE:
				glTextureStorage2DMultisample(renderer_id, parameters.samples, (GLenum)parameters.internal_format,
					parameters.width, parameters.height, (GLboolean)parameters.fixed_sample_locations);
				break;
			case TextureType::TEXTURE_3D_MULTISAMPLE:
				glTextureStorage3DMultisample(renderer_id, parameters.samples, (GLenum)parameters.internal_format,
					parameters.width, parameters.height, parameters.depth, (GLboolean)parameters.fixed_sample_locations);
				break;
			}
		}

		// Sets a section/all of the buffers data
		void Texture::SetTextureData(const TextureDataParameters& parameters, void* data) {
			switch (parameters.texture_type) {
			case TextureType::TEXTURE_1D:
				glTextureSubImage1D(renderer_id, 0, parameters.width_offset,
					parameters.width,
					(GLenum)parameters.base_format, (GLenum)parameters.data_type, data);
				break;
			case TextureType::TEXTURE_2D:
				[[fallthrough]];
			case TextureType::TEXTURE_1D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_2D_MULTISAMPLE:
				glTextureSubImage2D(renderer_id, 0, parameters.width_offset, parameters.height_offset,
					parameters.width, parameters.height,
					(GLenum)parameters.base_format, (GLenum)parameters.data_type, data);
				break;
			case TextureType::TEXTURE_3D:
				[[fallthrough]];
			case TextureType::TEXTURE_2D_ARRAY:
				[[fallthrough]];
			case TextureType::TEXTURE_3D_MULTISAMPLE:
				glTextureSubImage3D(renderer_id, 0, parameters.width_offset, parameters.height_offset, parameters.depth_offset,
					parameters.width, parameters.height, parameters.depth,
					(GLenum)parameters.base_format, (GLenum)parameters.data_type, data);
				break;
			}
		}

		void Texture::GenerateMipmaps() {
			glGenerateTextureMipmap(renderer_id);
		}

		void Texture::SetTextureSetting(TextureSettings setting, int32_t value) {
			glTextureParameteri(renderer_id, (GLenum)setting, (GLint)value);
		}
		void Texture::SetTextureSetting(TextureSettings setting, float* value) {
			glTextureParameterfv(renderer_id, (GLenum)setting, (GLfloat*)value);
		}

		void Texture::BindTexture(uint32_t texture_unit) {
			glBindTextureUnit(texture_unit, renderer_id);
		}
		Texture::~Texture() {
			glDeleteTextures(1, &renderer_id);
		}
	}
}
