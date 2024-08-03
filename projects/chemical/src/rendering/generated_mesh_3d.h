#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "opengl/vertex_array.h"

namespace Chemical {

	class GeneratedMesh3D {
	public:
		struct Vertex {
			inline static OpenGL::VertexLayout vertexLayout;

			static void InitializeVertexLayout() {
				vertexLayout.AddAttribute(OpenGL::VertexAttribute(3, 0));
				vertexLayout.AddAttribute(OpenGL::VertexAttribute(3, 3 * sizeof(float)));
			}

			glm::vec3 position;
			glm::vec3 normal;

			Vertex(const glm::vec3& position, const glm::vec3& normal) :
				position(position), normal(normal) {}

			Vertex() = default;
		};

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		OpenGL::VertexArray vArray;
		OpenGL::ElementDrawInfo drawInfo;

		void GenerateMesh() {
			OpenGL::Buffer v_buffer;
			v_buffer.CreateImmutableBuffer(sizeof(Vertex) * vertices.size(), &vertices[0]);

			OpenGL::Buffer e_buffer;
			e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * indices.size(), &indices[0]);

			vArray.SetVertexBuffer(v_buffer, Vertex::vertexLayout, 0, 0);
			vArray.SetElementBuffer(e_buffer);

			drawInfo.count = static_cast<int32_t>(indices.size());
		}

		GeneratedMesh3D(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
			vertices(vertices), indices(indices) {}
		GeneratedMesh3D() = default;
	};
}
