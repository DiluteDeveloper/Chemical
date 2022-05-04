#include "time.h"
#include "window.h"\

#include <GLFW/glfw3.h>

double Time::oldTime = 0;
double Time::deltaTime = 0;

void Time::init() {
	oldTime = glfwGetTime();
}

void Time::update() {
	double newTime = glfwGetTime();
	deltaTime = newTime - oldTime;
	oldTime = newTime;
}