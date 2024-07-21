#include <pch.h>

#include "init_flags.h"

namespace Chemical {

	namespace InitFlags {
		unsigned int WINDOW_INITIALIZED = 1 << 0;
		unsigned int GLFW_INITIALIZED = 1 << 1;
		unsigned int GLAD_INITIALIZED = 1 << 2;
		unsigned int LOGGER_INITIALIZED = 1 << 3;

		unsigned int flags = 0x00;

		extern void SetFlags(unsigned int f_flags) {
			flags |= f_flags;
		}

		extern bool HasFlags(unsigned int f_flags) {
			return (flags & f_flags) == f_flags;
		}
	};


}