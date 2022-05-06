#pragma once

#include "engine/renderer.h"

class Game {

	Renderer renderer;
	Scene scene;

	void keyPress(int key, int scancode, int action, int mods);
	void windowResize(int x, int y);

	bool overlaySelected = false;

public:
	Game();
	void update();
};
