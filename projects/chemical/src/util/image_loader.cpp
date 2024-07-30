#include <pch.h>

#include "image_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


namespace Chemical {

	namespace Util {

		std::optional<ImageData> LoadImage(std::string_view file_path, int32_t req_comp) {

			ImageData id;
			id.data = stbi_load(file_path.data(), &id.x, &id.y, &id.bit_depth, req_comp);
			if (!id.data) {
				spdlog::error("LoadImage failed with file path {0}", file_path);
				return std::nullopt;
			}

			return id;
		}
	}
}

