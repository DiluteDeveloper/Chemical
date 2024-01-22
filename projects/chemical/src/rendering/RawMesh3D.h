#pragma once

#include "graphics/opengl/VertexArray.h"

namespace Chemical {
	namespace Rendering {

		struct RawMesh3D {

			OpenGL::VertexArray vArray;
			OpenGL::Buffer vBuffer;
			OpenGL::Buffer eBuffer;
			OpenGL::ElementDrawInfo info;


			// NEED TO SETUP vArray and vBuffer
			RawMesh3D(const OpenGL::ElementDrawInfo& info) :
				info(info) {}
		};
	}
}
