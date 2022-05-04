#pragma once

struct GLFWwindow;

#include <glm/glm.hpp>

class Window {

	static GLFWwindow* glfwWindow;

public:

	static void init();
	static void initGUI();

	static glm::vec2 getSize();

	static void swapBuffers();

	static void setInputMode(int a, int b);

	static bool getKey(int key);
	static glm::vec2 getCursorPos();

	static bool close();
};