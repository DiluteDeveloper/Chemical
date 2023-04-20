#pragma once

#include "scene/scene.h"
#include "opengl/shader_program.h"
#include "opengl/buffer.h"

class Renderer {
	std::vector<OpenGL::ShaderProgram> shaderProgram;

	// Buffers that are kept alive for the duration of the renderer.
	// designed for buffer-backed interface blocks such as UBO's or SSBO's
	// Idea of not mapping to bindingIndex is that you should always hold onto
	// a reference to the buffer you want to edit and never have a reason to delete them.
	//  the only reason this exists is to hold onto your renderer-wide buffers.
	std::vector<OpenGL::Buffer> m_livingBuffers;

public:
	void InitializeRenderer();

	void SceneRenderer(const Scene& scene);

	OpenGL::Buffer& CreateLivingBuffer();
};
