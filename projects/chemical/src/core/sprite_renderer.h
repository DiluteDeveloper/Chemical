#pragma once

#include "nodes/sprite_2D.h"
namespace Chemical {
	namespace Core {
		class SpriteRenderer {

		public:
			void InitializeSpriteRenderer();
			void RenderSprites(const std::vector<Node::Sprite2D>& sprites);
		};
	}
}
