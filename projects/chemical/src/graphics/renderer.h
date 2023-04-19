#pragma once

#include "scene/scene.h"
#include "opengl/shader_program.h"

class Renderer {
	static std::unique_ptr<OpenGL::ShaderProgram> shaderProgram;

public:
	static void InitializeRenderer();

	static void SceneRenderer(const Scene& scene);
};
