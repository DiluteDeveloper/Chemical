#pragma once

namespace Chemical {

	namespace InitFlags {
		extern unsigned int WINDOW_INITIALIZED;
		extern unsigned int GLFW_INITIALIZED;
		extern unsigned int GLAD_INITIALIZED;
		extern unsigned int LOGGER_INITIALIZED;

		extern void SetFlags(unsigned int f_flags);
		extern bool HasFlags(unsigned int f_flags);
	};


}