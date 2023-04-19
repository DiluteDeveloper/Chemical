#pragma once

namespace Util {
	struct ImageData {
		int32_t x = 0;
		int32_t y = 0;
		int32_t bitDepth = 0;
		unsigned char* data = nullptr;
		ImageData(int32_t x, int32_t y, int32_t bitDepth, unsigned char* data) :
			x(x), y(y), bitDepth(bitDepth), data(data) {}
		ImageData() {}
	};

	// Always outputs a bit depth of 8
	extern ImageData LoadImage(std::string_view filePath, int32_t req_comp = 0);

}

