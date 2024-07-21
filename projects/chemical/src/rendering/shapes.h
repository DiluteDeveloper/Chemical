#pragma once

#include "graphics/opengl/vertex_array.h"
namespace Chemical {
	namespace Shapes {

		extern OpenGL::ElementDrawInfo info;
		extern std::unique_ptr<OpenGL::VertexArray> cube;
		extern std::unique_ptr<OpenGL::VertexArray> pyramid;
		
		extern void InitializeShapes();

	}
}
