#pragma once

namespace Chemical {
	
	class GLFWCallbackReceiver {
	public:
		virtual void WindowCloseCallback(GLFWwindow* window) {}
		virtual void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {}
	};

	class GLFWWrapper {
	public:

		std::string glsl_version = "#version 460";
		GLFWwindow* window = nullptr;
		GLFWWrapper(unsigned int f_window_size_x = 720, unsigned int f_window_size_y = 480);

		unsigned int GetWindowSizeX() const;
		unsigned int GetWindowSizeY() const;

		static void SetStaticCallbackReceiver(GLFWCallbackReceiver* receiver);
	private:
		static void WindowCloseCallback(GLFWwindow* window);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static GLFWCallbackReceiver* receiver;

		unsigned int window_size_x = 0;
		unsigned int window_size_y = 0;
	};
}
