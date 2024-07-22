#pragma once


namespace Chemical {
	namespace Util {
		struct ImageData {
			int32_t x = 0;
			int32_t y = 0;
			int32_t bit_depth = 0;
			unsigned char* data = nullptr;
			ImageData(int32_t x, int32_t y, int32_t bit_depth, unsigned char* data) :
				x(x), y(y), bit_depth(bit_depth), data(data) {}
			ImageData() {}
		};

		// Always outputs a bit depth of 8
		extern std::optional<ImageData> LoadImage(std::string_view file_path, int32_t req_comp = 0);

	}


}


