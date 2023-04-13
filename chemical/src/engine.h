#pragma once

struct GLFWwindow;

class Engine {
	friend int main();
	GLFWwindow* window;
	

	Engine();

	void Update();


};
