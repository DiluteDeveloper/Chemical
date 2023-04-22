#pragma once

// shader agnostic meshes

#include "util/typeless_array.h"

class StaticMesh {
	friend class Renderer;
	Util::TypelessArray vertexData;
	Util::TypelessArray elementData;

public:

	StaticMesh(const Util::TypelessArray& vertexData, const Util::TypelessArray& elementData) :
		vertexData(vertexData), elementData(elementData) {}
};
