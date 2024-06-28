#include "pch.h"
#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


namespace Chemical {

	namespace Util {

		ImageData LoadImageFromPath(std::string_view filePath, int32_t requiredChannels) {

			ImageData id;
			id.data = stbi_load(filePath.data(), &id.x, &id.y, &id.channels, requiredChannels);
			return id;
		}
	}
}

