#pragma once

#include "graphics/opengl/vertex_array.h"

#include "node_2D.h"

namespace Chemical {
	
	namespace Node {
		class Sprite2D {
			// For now just represents a blank square, no texturing
			// For now is implementing its own OpenGL Mesh
		public:
			Node2D node_2D;
			unsigned int zIndex = 0;
			static void InitializeSprite2D(); // static and called once for all sprites
			static std::unique_ptr<OpenGL::VertexArray> vArray; // square
			static OpenGL::ElementDrawInfo info;
			static OpenGL::VertexLayout layout;
		};
	}
}