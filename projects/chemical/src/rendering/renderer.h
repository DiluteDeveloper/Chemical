#pragma once

#include "graphics/opengl/vertex_array.h"
#include "data_types/mesh.h"

namespace Chemical {
	
	class Renderer3D {
	public:
		static void AddMeshToRender(const Mesh& mesh);

		static void InitializeRenderer();

		static void Render();

	private:
		static std::vector<OpenGL::VertexArray> v_arrays;
		static std::vector<OpenGL::ElementDrawInfo> v_infos;
	};
}
