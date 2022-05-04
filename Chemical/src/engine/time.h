#pragma once

class Time {

	static double oldTime;

public:

	static void init();
	static void update();

	static double deltaTime;
};