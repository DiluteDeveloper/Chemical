#pragma once

#include <glm/glm.hpp>

#include "rendering/opengl/misc.h"
#include "rendering/opengl/vertex_array.h"
#include "object.h"
#include "object_3d.h"

namespace Chemical {

	namespace Scene {
		class Mesh3D {
		public:
			struct Vertex {
				inline static OpenGL::VertexLayout vertex_layout;

				static void InitializeVertexLayout() {
					vertex_layout.AddAttribute(OpenGL::VertexAttribute(3, 0));
					vertex_layout.AddAttribute(OpenGL::VertexAttribute(3, 3 * sizeof(float)));
				}

				glm::vec3 position;
				glm::vec3 normal;

				Vertex(const glm::vec3& position, const glm::vec3& normal) :
					position(position), normal(normal) {}

				Vertex() = default;
			};

			Object object;
			Object3D object_3d;

			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			OpenGL::VertexArray v_array;
			OpenGL::ElementDrawInfo draw_info;

			void GenerateMesh() {
				OpenGL::Buffer v_buffer;
				v_buffer.CreateImmutableBuffer(sizeof(Vertex) * vertices.size(), &vertices[0]);

				OpenGL::Buffer e_buffer;
				e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * indices.size(), &indices[0]);

				v_array.SetVertexBuffer(v_buffer, Vertex::vertex_layout, 0, 0);
				v_array.SetElementBuffer(e_buffer);

				draw_info.count = static_cast<int32_t>(indices.size());
			}

			Mesh3D(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) :
				vertices(vertices), indices(indices), object(this, "Mesh3D") {}

			Mesh3D() : object(this, "Mesh3D") {}

		};
	}

}

