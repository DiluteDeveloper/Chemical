#pragma once


namespace Chemical {
	namespace Util {
		struct ImageData {
			int32_t x = 0;
			int32_t y = 0;

			// Is set to channels of this image data, not of the original image, the channels could be modified 
			// to suit needs. STBI discourages this but why would i want the number of original image channels if im forcing them to be different?
			int32_t channels = 0;

			void* data = nullptr;
			ImageData(int32_t x, int32_t y, int32_t channels, void* data) :
				x(x), y(y), channels(channels), data(data) {}
			ImageData() {}
		};

		// Always outputs a bit depth of 8
		extern ImageData LoadImageFromPath(std::string_view file_path, int32_t forced_channels = 0);

	}


}


