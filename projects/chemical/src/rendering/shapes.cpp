#include <pch.h>

#include "shapes.h"

namespace Chemical {
	namespace Shapes {
		OpenGL::VertexLayout layout;
		OpenGL::ElementDrawInfo info;

		std::unique_ptr<OpenGL::VertexArray> cube;
		std::unique_ptr<OpenGL::VertexArray> pyramid;

		void InitializeCube() {

			cube = std::make_unique<OpenGL::VertexArray>();

			// comments are from the perspective of the face normal
			const float vertices[] = {

				// BACK FACE
				0.5f, 0.5f, -0.5f, 0, 0, -1,   // TOP RIGHT (RELATIVE TO EACH FACE)
				-0.5f, 0.5f, -0.5f, 0, 0, -1,  // TOP LEFT
				-0.5f, -0.5f, -0.5f, 0, 0, -1, // BOTTOM LEFT
				0.5f, -0.5f, -0.5f, 0, 0, -1,  // BOTTOM RIGHT

				// FRONT FACE
				-0.5f, 0.5f, 0.5f, 0, 0, 1,    // TOP RIGHT (RELATIVE TO EACH FACE)
				0.5f, 0.5f, 0.5f, 0, 0, 1,	   // TOP LEFT
				0.5f, -0.5f, 0.5f, 0, 0, 1,	   // BOTTOM LEFT
				-0.5f, -0.5f, 0.5f, 0, 0, 1,   // BOTTOM RIGHT

				// LEFT FACE
				-0.5f, 0.5f, -0.5f, -1, 0, 0,  // TOP RIGHT (RELATIVE TO EACH FACE)
				-0.5f, 0.5f, 0.5f, -1, 0, 0,   // TOP LEFT
				-0.5f, -0.5f, 0.5f, -1, 0, 0,  // BOTTOM LEFT
				-0.5f, -0.5f, -0.5f, -1, 0, 0, // BOTTOM RIGHT

				// RIGHT FACE
				0.5f, 0.5f, 0.5f, 1, 0, 0,     // TOP RIGHT (RELATIVE TO EACH FACE)
				0.5f, 0.5f, -0.5f, 1, 0, 0,    // TOP LEFT
				0.5f, -0.5f, -0.5f, 1, 0, 0,   // BOTTOM LEFT
				0.5f, -0.5f, 0.5f, 1, 0, 0,    // BOTTOM RIGHT

				// TOP FACE
				0.5f, 0.5f, 0.5f, 0, 1, 0,     // TOP RIGHT (RELATIVE TO EACH FACE)
				-0.5f, 0.5f,0.5f, 0, 1, 0,     // TOP LEFT
				-0.5f, 0.5f, -0.5f, 0, 1, 0,   // BOTTOM LEFT
				0.5f, 0.5f, -0.5f, 0, 1, 0,    // BOTTOM RIGHT

				// BOTTOM FACE
				0.5f, -0.5f, -0.5f, 0, -1, 0,  // TOP RIGHT (RELATIVE TO EACH FACE)
				-0.5f, -0.5f,-0.5f, 0, -1, 0,  // TOP LEFT
				-0.5f, -0.5f, 0.5f, 0, -1, 0,  // BOTTOM LEFT
				0.5f, -0.5f, 0.5f, 0, -1, 0    // BOTTOM RIGHT
			};

			const unsigned int indices[] = {
				0 + (4 * 0), 1 + (4 * 0),  2 + (4 * 0),
				2 + (4 * 0), 3 + (4 * 0), 0 + (4 * 0), // BACK FACE

				0 + (4 * 1), 1 + (4 * 1),  2 + (4 * 1),
				2 + (4 * 1), 3 + (4 * 1), 0 + (4 * 1), // FRONT FACE

				0 + (4 * 2), 1 + (4 * 2),  2 + (4 * 2),
				2 + (4 * 2), 3 + (4 * 2), 0 + (4 * 2), // LEFT FACE

				0 + (4 * 3), 1 + (4 * 3),  2 + (4 * 3),
				2 + (4 * 3), 3 + (4 * 3), 0 + (4 * 3), // RIGHT FACE

				0 + (4 * 4), 1 + (4 * 4),  2 + (4 * 4),
				2 + (4 * 4), 3 + (4 * 4), 0 + (4 * 4), // TOP FACE

				0 + (4 * 5), 1 + (4 * 5),  2 + (4 * 5),
				2 + (4 * 5), 3 + (4 * 5), 0 + (4 * 5) // BOTTOM FACE
			};

			OpenGL::Buffer v_buffer;
			v_buffer.CreateImmutableBuffer(sizeof(float) * 144, &vertices[0]);

			OpenGL::Buffer e_buffer;
			e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * 36, &indices[0]);

			cube->SetVertexBuffer(v_buffer, layout, 0, 0);
			cube->SetElementBuffer(e_buffer);

			info.count = 36;
		}

		void InitializePyramid() {

		}

		void InitializeShapes() {
			InitializeCube();
			InitializePyramid();
		}
	}
}