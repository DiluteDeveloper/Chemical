#pragma once

#include <cstdint>

#include <chemical/window.h>

namespace Chemical {

	namespace Graphics {

		struct RGBColour {
			uint8_t r = 255;
			uint8_t g = 255;
			uint8_t b = 255;

			RGBColour(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255) :
				r(r), g(g), b(b) {}
		};

		void SetBackgroundColour(RGBColour colour);

		void ClearColourBuffer();
		void SwapBuffers(Window window);
	}
}
