#include <pch.h>

#include "renderer.h"

#include "opengl/shader_program.h"
#include "util/filestream.h"
#include "core/glfw_glad/init_glfw_glad.h"
#include "camera.h"

#include "util/transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

	void Renderer3D::AddMeshToRender(const Mesh& mesh) {

		OpenGL::Buffer v_buffer;
		v_buffer.CreateImmutableBuffer(sizeof(Vertex) * mesh.vertices.size(), &mesh.vertices[0]);

		OpenGL::Buffer e_buffer;
		e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * mesh.indices.size(), &mesh.indices[0]);

		v_arrays.emplace_back();
		v_arrays[v_arrays.size() - 1].SetVertexBuffer(v_buffer, Vertex::vertex_layout, 0, 0);
		v_arrays[v_arrays.size() - 1].SetElementBuffer(e_buffer);

		v_infos.emplace_back();
		v_infos[v_infos.size() - 1].count = static_cast<int32_t>(mesh.indices.size());
	}

	Renderer3D::Renderer3D(const GLFWWrapper* f_wrapper) {

		wrapper = f_wrapper;
		Vertex::InitializeVertexLayout();

		Util::FileStream stream;
		
		std::string vertex_shader_source = stream.ReadFile("resources/shaders/test_shader.vert").value();
		std::string fragment_shader_source = stream.ReadFile("resources/shaders/test_shader.frag").value();

		if (QUERY_ERROR) {
			CHEMICAL_PRINT(Severity::_ERROR, "Error occurred reading shader source files in Renderer3D constructor, returning.");
			return;
		}

		OpenGL::Shader vertex_shader(vertex_shader_source.c_str(), OpenGL::ShaderType::VERTEX_SHADER);
		OpenGL::Shader fragment_shader(fragment_shader_source.c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

		program = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
			const OpenGL::Shader*>{ &vertex_shader, &fragment_shader }, Vertex::vertex_layout);

		// perspectiveLH converts left handed input data to opengls coordinate system
		projection = glm::perspectiveLH(glm::radians(90.0f), wrapper->GetWindowSizeX() / static_cast<float>(wrapper->GetWindowSizeY()), 0.1f, 1000.0f);
	}

	void Renderer3D::Render() {

		program->BindProgram();

		program->SetUniformMatrix4FV("projection", 1, false, &projection[0][0]);
		program->SetUniformMatrix4FV("view", 1, false, &glm::inverse(camera.UpdateMovement(wrapper))[0][0]);
		program->SetUniformMatrix4FV("model", 1, false, &model[0][0]);

		for (size_t i = 0; i < v_arrays.size(); i++)
		{
			v_arrays[i].Bind();
			v_arrays[i].DrawElements(v_infos[i]);
		}


	}
}
