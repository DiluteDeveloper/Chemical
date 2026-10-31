#pragma once

#include <cstdint>
#include <utility>

#include "misc.h"


namespace Chemical {


	namespace OpenGL {

		// Only parameters that exist in GLES 3.0 / WebGL2
		// (DEPTH_STENCIL_TEXTURE_MODE and TEXTURE_LOD_BIAS were removed).
		enum class TextureSettings {
			TEXTURE_BASE_LEVEL = 0x813C,
			TEXTURE_COMPARE_FUNC = 0x884D,
			TEXTURE_COMPARE_MODE = 0x884C,
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

		// Sized internal formats supported by GLES 3.0 glTexStorage* (16-bit normalized, RGB10/12,
		// RGBA12/16, R3_G3_B2 etc. are desktop-only and were removed).
		enum class TextureInternalFormat {
			R8 = 0x8229,
			R8_SNORM = 0x8F94,
			RG8 = 0x822B,
			RG8_SNORM = 0x8F95,
			RGB8 = 0x8051,
			RGB8_SNORM = 0x8F96,
			RGB565 = 0x8D62,
			RGBA4 = 0x8056,
			RGB5_A1 = 0x8057,
			RGBA8 = 0x8058,
			RGBA8_SNORM = 0x8F97,
			RGB10_A2 = 0x8059,
			RGB10_A2UI = 0x906F,
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
			RGBA32UI = 0x8D70,
			// depth / depth-stencil
			DEPTH_COMPONENT16 = 0x81A5,
			DEPTH_COMPONENT24 = 0x81A6,
			DEPTH_COMPONENT32F = 0x8CAC,
			DEPTH24_STENCIL8 = 0x88F0,
			DEPTH32F_STENCIL8 = 0x8CAD
		};

		// "Format" argument of glTexSubImage*. Integer internal formats (R8UI, RGBA32I, ...)
		// require the matching *_INTEGER base format. BGR and STENCIL_INDEX don't exist in GLES 3.0.
		enum class TextureBaseFormat {
			RED = 0x1903,
			RG = 0x8227,
			RGB = 0x1907,
			RGBA = 0x1908,
			RED_INTEGER = 0x8D94,
			RG_INTEGER = 0x8228,
			RGB_INTEGER = 0x8D98,
			RGBA_INTEGER = 0x8D99,
			DEPTH_COMPONENT = 0x1902,
			DEPTH_STENCIL = 0x84F9
		};

		// 1D, 1D array and multisample textures don't exist in GLES 3.0 / WebGL2
		// (use renderbuffers for MSAA).
		enum class TextureType {
			TEXTURE_2D = 0x0DE1,
			TEXTURE_2D_ARRAY = 0x8C1A,
			TEXTURE_3D = 0x806F
		};

		struct TextureStorageParameters {

			TextureInternalFormat internalFormat = TextureInternalFormat::SRGB8;
			TextureType textureType = TextureType::TEXTURE_2D;

			int32_t width = 0;
			int32_t height = 0;
			int32_t depth = 0; // TEXTURE_3D depth, or layer count for TEXTURE_2D_ARRAY

			int32_t mipLevels = 1;

			TextureStorageParameters() = default;
		};

		// The texture type is remembered from creation, so it is no longer part of this struct.
		// Extra data will be ignored.
		struct TextureDataParameters {
			TextureBaseFormat baseFormat = TextureBaseFormat::RGB;
			DataType dataType = DataType::UNSIGNED_BYTE;

			int32_t mipLevel = 0;

			int32_t widthOffset = 0;
			int32_t heightOffset = 0;
			int32_t depthOffset = 0; // layer offset for 2D arrays

			int32_t width = 0;
			int32_t height = 0;
			int32_t depth = 0; // layer count for 2D arrays

			TextureDataParameters() = default;
		};

		// storage size is immutable
		// Recommended to make a shared_ptr as copy constructor is disabled
		//
		// NOTE: without DSA, modifying calls (SetTextureData, GenerateMipmaps, SetTextureSetting)
		// bind the texture to the *currently active* texture unit.
		class Texture {
			uint32_t rendererID = 0;
			TextureType type = TextureType::TEXTURE_2D;

			void Release();
		public:

			Texture(const TextureStorageParameters& parameters);
			Texture(Texture&& other) noexcept :
				rendererID(std::exchange(other.rendererID, 0)), type(other.type) {}

			Texture(const Texture&) = delete;
			Texture& operator=(const Texture&) = delete;
			Texture& operator=(Texture&& other) noexcept {
				if (this != &other) {
					Release();
					rendererID = std::exchange(other.rendererID, 0);
					type = other.type;
				}
				return *this;
			}

			~Texture();

			// Sets a section/all of the texture's data
			void SetTextureData(const TextureDataParameters& parameters, const void* data = nullptr);

			void GenerateMipmaps();

			// Set opengl texture parameter values
			void SetTextureSetting(TextureSettings setting, int32_t value);
			// Set opengl texture parameter values
			void SetTextureSetting(TextureSettings setting, const float* value);

			// helper function, binds texture to textureUnit slot on shader
			void BindTexture(uint32_t textureUnit);

			uint32_t GetRendererID() const {
				return rendererID;
			}
			TextureType GetTextureType() const {
				return type;
			}
		};
	}

}
