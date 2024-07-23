#pragma once

#include "opengl/vertex_array.h"
#include "opengl/shader_program.h"
#include "camera.h"
#include "data_types/mesh.h"
#include "util/filestream.h"
#include "core/glfw_glad/init_glfw_glad.h"

namespace Chemical {
	
	class Renderer3D {
	public:
		Renderer3D(const GLFWWrapper* f_wrapper);

		void AddMeshToRender(const Mesh& mesh);

		void Render();

	private:
		const GLFWWrapper* wrapper = nullptr;
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
