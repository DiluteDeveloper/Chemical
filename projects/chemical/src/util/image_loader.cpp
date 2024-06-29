#include "pch.h"
#include "image_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


namespace Chemical {

	namespace Util {

		ImageData LoadImageFromPath(std::string_view file_path, int32_t forced_channels) {

			ImageData id;
			id.data = stbi_load(file_path.data(), &id.x, &id.y, &id.channels, forced_channels);

			if (forced_channels != 0)
				id.channels = forced_channels;
			return id;
		}
	}
}

