#pragma once

#include "opengl/vertex_array.h"
#include "opengl/shader_program.h"
#include "camera.h"
#include "data_types/mesh.h"
#include "util/filestream.h"

namespace Chemical {
	
	class Renderer3D {
	public:
		Renderer3D();

		void AddMeshToRender(const Mesh& mesh);

		void Render();

	private:
		Camera camera;

		std::vector<OpenGL::VertexArray> v_arrays;
		std::vector<OpenGL::ElementDrawInfo> v_infos;

		glm::mat4 view = glm::mat4(1.0f);
		OpenGL::VertexLayout layout;

		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);

		std::unique_ptr<OpenGL::ShaderProgram> program;
	};
}
