#pragma once

namespace Chemical {

	extern GLFWwindow* window;
	extern bool InitializeGLFWGLAD(unsigned int f_window_size_x = 720, unsigned int f_window_size_y = 480);

	extern unsigned int GetWindowSizeX();
	extern unsigned int GetWindowSizeY();
}
