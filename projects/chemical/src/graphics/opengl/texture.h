#pragma once

#include "misc.h"

namespace OpenGL {

	enum class TextureSettings {
		DEPTH_STENCIL_TEXTURE_MODE = 0x90EA,
		TEXTURE_BASE_LEVEL = 0x813C,
		TEXTURE_COMPARE_FUNC = 0x884D,
		TEXTURE_COMPARE_MODE = 0x884C,
		TEXTURE_LOD_BIAS = 0x8501,
		TEXTURE_MIN_FILTER = 0x2801,
		TEXTURE_MAG_FILTER = 0x2800,
		TEXTURE_MIN_LOD = 0x813A,
		TEXTURE_MAX_LOD = 0x813B,
		TEXTURE_MAX_LEVEL = 0x813D,
		TEXTURE_SWIZZLE_R = 0x8E42,
		TEXTURE_SWIZZLE_G = 0x8E43,
		TEXTURE_SWIZZLE_B = 0x8E44,
		TEXTURE_SWIZZLE_A = 0x8E45,
		TEXTURE_WRAP_S = 0x2802,
		TEXTURE_WRAP_T = 0x2803,
		TEXTURE_WRAP_R = 0x8072

	};
	enum class TextureInternalFormat {
		R8 = 0x8229,
		R8_SNORM = 0x8F94,
		R16 = 0x822A,
		R16_SNORM = 0x8F98,
		RG8 = 0x822B,
		RG8_SNORM = 0x8F95,
		RG16 = 0x822C,
		RG16_SNORM = 0x8F99,
		R3_G3_B2 = 0x2A10,
		RGB4 = 0x804F,
		RGB5 = 0x8050,
		RGB8 = 0x8051,
		RGB8_SNORM = 0x8F96,
		RGB10 = 0x8052,
		RGB12 = 0x8053,
		RGB16_SNORM = 0x8F9A,
		RGBA2 = 0x8055,
		RGBA4 = 0x8056,
		RGB5_A1 = 0x8057,
		RGBA8 = 0x8058,
		RGBA8_SNORM = 0x8F97,
		RGB10_A2 = 0x8059,
		RGB10_A2UI = 0x906F,
		RGBA12 = 0x805A,
		RGBA16 = 0x805B,
		SRGB8 = 0x8C41,
		SRGB8_ALPHA8 = 0x8C43,
		R16F = 0x822D,
		RG16F = 0x822F,
		RGB16F = 0x881B,
		RGBA16F = 0x881A,
		R32F = 0x822E,
		RG32F = 0x8230,
		RGB32F = 0x8815,
		RGBA32F = 0x8814,
		R11F_G11F_B10F = 0x8C3A,
		RGB9_E5 = 0x8C3D,
		R8I = 0x8231,
		R8UI = 0x8282,
		R16I = 0x8233,
		R16UI = 0x8234,
		R32I = 0x8235,
		R32UI = 0x8236,
		RG8I = 0x8237,
		RG8UI = 0x8238,
		RG16I = 0x8239,
		RG16UI = 0x823A,
		RG32I = 0x823B,
		RG32UI = 0x823C,
		RGB8I = 0x8D8F,
		RGB8UI = 0x8D7D,
		RGB16I = 0x8D89,
		RGB16UI = 0x8D77,
		RGB32I = 0x8D83,
		RGB32UI = 0x8D71,
		RGBA8I = 0x8D8E,
		RGBA8UI = 0x8D7C,
		RGBA16I = 0x8D88,
		RGBA16UI = 0x8D76,
		RGBA32I = 0x8D82,
		RGBA32UI = 0x8D70
	};
	enum class TextureBaseFormat {
		RED = 0x1903,
		RG = 0x8227,
		RGB = 0x1907,
		BGR = 0x80E0,
		RGBA = 0x1908,
		DEPTH_COMPONENT = 0x1902,
		STENCIL_INDEX = 0x1901
	};

	enum class TextureType {
		TEXTURE_1D = 0x0DE0,
		TEXTURE_1D_ARRAY = 0x8C18,
		TEXTURE_2D = 0x0DE1,
		TEXTURE_2D_ARRAY = 0x8C1A,
		TEXTURE_3D = 0x806F,
		TEXTURE_2D_MULTISAMPLE = 0x9100,
		TEXTURE_3D_MULTISAMPLE = 0x9102
	};

	//uint16_t ConvertTextureSizedInternalFormatToBaseInternalFormat(TextureSizedInternalFormat sizedInternalFormat);

	struct TextureStorageParameters {

		TextureInternalFormat internalFormat = TextureInternalFormat::SRGB8;
		TextureType textureType = TextureType::TEXTURE_2D;

		int32_t width = 0;
		int32_t height = 0;
		int32_t depth = 0;

		int32_t mipLevels = 1;

		// for multisample
		int32_t samples = 0;
		bool fixedSampleLocations = false;

		TextureStorageParameters() = default;


	};

	// Extra data will be ignored
	struct TextureDataParameters {
		TextureBaseFormat baseFormat = TextureBaseFormat::RGB;
		DataType dataType = DataType::UNSIGNED_BYTE;
		TextureType textureType = TextureType::TEXTURE_2D;

		int32_t widthOffset = 0;
		int32_t heightOffset = 0;
		int32_t depthOffset = 0;

		int32_t width = 0;
		int32_t height = 0;
		int32_t depth = 0;

		TextureDataParameters() = default;
	};
	// storage size is immutable
	// Recommended to make a shared_ptr as copy constructor is disabled
	class Texture {
		uint32_t m_rendererID = 0;

	public:

		Texture(const TextureStorageParameters& parameters);
		Texture(Texture&& other) noexcept :
			m_rendererID(std::move(other.m_rendererID)) {}

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&& other) noexcept {
			this->m_rendererID = std::move(other.m_rendererID);
			return *this;
		}

		~Texture();

		// Sets a section/all of the buffers data
		void SetTextureData(const TextureDataParameters& parameters, void* data = nullptr);

		void GenerateMipmaps();

		// Set opengl texture parameter values
		void SetTextureSetting(TextureSettings setting, int32_t value);
		// Set opengl texture parameter values
		void SetTextureSetting(TextureSettings setting, float* value);

		// helper function, binds texture buffer to textureUnit slot on shader
		void BindTexture(uint32_t textureUnit);

		uint32_t GetRendererID() const {
			return m_rendererID;
		}
	};
}
