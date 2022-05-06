#pragma once

#include "shader.h"
#include "scene.h"

class Renderer {

	Shader litShader;
	Shader unlitShader;

	Scene* scene = nullptr;

public:

	Renderer(Scene& in_scene);
	Renderer();

	void bindScene(Scene& in_scene);

	void render() const;
	void renderSceneGUI() const;
};
