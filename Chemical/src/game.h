#pragma once

#include "engine/shader.h"
#include "engine/camera.h"
#include "object.h"

class Game {
	Transform light;
	Shader textureShader;
	Shader colourShader;
	Camera camera;

	Object box;
	Object floor;
	Mesh* cube = nullptr;

	glm::mat4* proj = nullptr;
	glm::vec3 lightColour = glm::vec3(1.0f);

	void keyPress(int key, int scancode, int action, int mods);
	void windowResize(int x, int y);

	bool overlaySelected = false;

	void updateGUI();

public:
	Game();
	void update();
	~Game();
};
