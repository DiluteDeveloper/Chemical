#include <pch.h>

#include <GLAD/glad.h>

#include "sprite_2D.h"

namespace Chemical {
	namespace Node {

		void Sprite2D::SetImageTexture(const Util::ImageData& image) {

#ifdef CHEMICAL_DEBUG
			if (image.channels != 4)
				LOGGER_CONSOLE_CUSTOM_ERROR("Sprite2D::SetImageTexture() Received image with {} channels. 4 channels is required.", image.channels);
#endif

			image_scale = glm::ivec2(image.x, image.y);

			OpenGL::TextureStorageParameters p;
			p.internal_format = OpenGL::TextureInternalFormat::RGBA8;
			p.width = image.x;
			p.height = image.y;
			image_texture = std::make_unique<OpenGL::Texture>(OpenGL::Texture(p));

			OpenGL::TextureDataParameters param;
			param.base_format = OpenGL::ConvertInternalToBase(p.internal_format);
			param.data_type = OpenGL::DataType::UNSIGNED_BYTE;
			param.width = image.x;
			param.height = image.y;
			param.texture_type = OpenGL::TextureType::TEXTURE_2D;

			image_texture->SetTextureData(param, image.data);
			image_texture->GenerateMipmaps();

			image_texture->SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
			image_texture->SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}