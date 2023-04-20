#pragma once

// shader agnostic meshes

#include "util/typeless_array.h"
#include "graphics/opengl/vertex_array.h"

class StaticMeshData {
	friend class StaticMesh;
	OpenGL::VertexBufferInfo m_vertexInfo;

	Util::TypelessArray m_vertexData;
	Util::TypelessArray m_elementData;

	OpenGL::ElementDrawInfo m_drawInfo;
public:

	StaticMeshData(const Util::TypelessArray& vertexData, const Util::TypelessArray& elementData);

	void SetVertexData(const Util::TypelessArray& data);
	void SetElementData(const Util::TypelessArray& data);

	StaticMeshData();

	void AddVertexAttribute(const OpenGL::VertexAttribute attribute);

	void SetDrawMode(OpenGL::DrawMode mode);
};

// stores elements and vertices in 1 buffer, can be used for many meshes in one
class StaticMesh {
	OpenGL::VertexArray m_vArray;

	OpenGL::ElementDrawInfo m_info;
public:

	void SetDrawMode(OpenGL::DrawMode mode);

	StaticMesh(const StaticMeshData& data);

	void Draw();
};
