#include <pch.h>

#include "mesh.h"

namespace Chemical {
	void Mesh::GenerateMesh() {

		OpenGL::Buffer v_buffer;
		v_buffer.CreateImmutableBuffer(sizeof(Vertex) * vertices.size(), &vertices[0]);

		OpenGL::Buffer e_buffer;
		e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * indices.size(), &indices[0]);

		v_array.SetVertexBuffer(v_buffer, Vertex::vertex_layout, 0, 0);
		v_array.SetElementBuffer(e_buffer);

		draw_info.count = static_cast<int32_t>(indices.size());
	}
}