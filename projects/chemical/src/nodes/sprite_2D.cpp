#include <pch.h>

#include <GLAD/glad.h>

#include "sprite_2D.h"

namespace Chemical {
	namespace Node {

		void Sprite2D::SetImageTexture(const Util::ImageData& image) {
			// SPRITES CURRENTLY REQUIRE 4 CHANNELS

			image_scale = glm::ivec2(image.x, image.y);

			OpenGL::TextureStorageParameters p;
			p.internalFormat = OpenGL::TextureInternalFormat::RGBA8;
			p.width = image.x;
			p.height = image.y;
			imageTexture = std::make_unique<OpenGL::Texture>(OpenGL::Texture(p));

			OpenGL::TextureDataParameters param;
			param.baseFormat = OpenGL::TextureBaseFormat::RGBA;
			param.dataType = OpenGL::DataType::UNSIGNED_BYTE;
			param.width = image.x;
			param.height = image.y;
			param.textureType = OpenGL::TextureType::TEXTURE_2D;

			imageTexture->SetTextureData(param, image.data);
			imageTexture->GenerateMipmaps();

			imageTexture->SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
			imageTexture->SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);
		}
	}
}