#pragma once


namespace Chemical {
	namespace Util {
		struct ImageData {
			int32_t x = 0;
			int32_t y = 0;
			int32_t channels = 0;
			void* data = nullptr;
			ImageData(int32_t x, int32_t y, int32_t channels, void* data) :
				x(x), y(y), channels(channels), data(data) {}
			ImageData() {}
		};

		// Always outputs a bit depth of 8
		extern ImageData LoadImageFromPath(std::string_view filePath, int32_t requiredChannels = 0);

	}


}


