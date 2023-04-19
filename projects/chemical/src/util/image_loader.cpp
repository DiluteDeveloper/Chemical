#include "pch.h"
#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace Util {

	ImageData LoadImage(std::string_view filePath, int32_t req_comp) {

		ImageData id;
		id.data = stbi_load(filePath.data(), &id.x, &id.y, &id.bitDepth, req_comp);
		return id;
	}
}