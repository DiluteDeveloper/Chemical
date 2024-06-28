#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/texture.h"

#include "node_2D.h"

#include "util/image_loader.h"

namespace Chemical {
	
	namespace Node {
		class Sprite2D {
			// For now is implementing its own OpenGL Texture
			// 
			// only supports png currently
		public:
			void SetImageTexture(const Util::ImageData& image);
			std::unique_ptr<OpenGL::Texture> imageTexture;
			glm::ivec2 image_scale = glm::ivec2(0); // num of pixels on image texture
			Node2D node_2D;
			int zIndex = 0;
		};
	}
}