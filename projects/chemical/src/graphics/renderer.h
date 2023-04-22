#pragma once

#include "scene/scene.h"
#include "opengl/shader_program.h"
#include "opengl/vertex_array.h"


struct SceneRenderData {
	OpenGL::VertexArray staticVArray;
	uint32_t count = 0;

	SceneRenderData() = default;
};

class Renderer {
	std::unordered_map<HashedString, std::shared_ptr<OpenGL::ShaderProgram>> m_shaders;

	// Buffers that are kept alive for the duration of the renderer.
	// designed for buffer-backed interface blocks such as UBO's or SSBO's
	// Idea of not mapping to bindingIndex is that you should always hold onto
	// a reference to the buffer you want to edit and never have a reason to delete them.
	//  the only reason this exists is to hold onto your renderer-wide buffers.
	//std::vector<OpenGL::Buffer> m_livingBuffers;

	std::unordered_map<HashedString, SceneRenderData> m_sceneRenderData;

public:

	Renderer() {}

	void GenerateSceneRenderData(const Scene& scene);

	void RenderSceneData();

	void AddShader(HashedString h, const std::shared_ptr<OpenGL::ShaderProgram> program);
};

