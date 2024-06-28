#include <pch.h>

#include "sprite_2D.h"

namespace Chemical {
	namespace Node {

		std::unique_ptr<OpenGL::VertexArray> Sprite2D::vArray;
		OpenGL::ElementDrawInfo Sprite2D::info;
		OpenGL::VertexLayout Sprite2D::layout;

		void Sprite2D::InitializeSprite2D() {
			OpenGL::Buffer vBuffer;

			vArray = std::make_unique<OpenGL::VertexArray>();

			float vertices[] = {
				-0.5f, -0.5f,
				-0.5f, 0.5f,
				0.5f, 0.5f,
				0.5f, -0.5f
			};
			vBuffer.CreateImmutableBuffer(sizeof(float) * 8, &vertices[0]);
			unsigned int indices[] = {
				2, 1, 0,
				2, 0, 3
			};
			OpenGL::Buffer eBuffer;
			eBuffer.CreateImmutableBuffer(sizeof(unsigned int) * 6, &indices[0]);

			layout.AddAttribute(OpenGL::VertexAttribute(2, 0, OpenGL::DataType::FLOAT));
			info.count = 6;
			info.dataType = OpenGL::DataType::UNSIGNED_INT;
			info.mode = OpenGL::DrawMode::TRIANGLES;
			info.offset = 0;

			vArray->SetVertexBuffer(vBuffer, layout, 0, 0);
			vArray->SetElementBuffer(eBuffer);
		}
	}
}