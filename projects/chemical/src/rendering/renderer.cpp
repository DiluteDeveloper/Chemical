#include <pch.h>

#include "renderer.h"

#include "opengl/shader_program.h"
#include "util/filestream.h"
#include "core/glfw_glad/init_glfw_glad.h"
#include "camera.h"

#include "util/transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Chemical {

	Renderer3D::Renderer3D(const GLFWWrapper* f_wrapper) {

		wrapper = f_wrapper;
		Vertex::InitializeVertexLayout();

		Util::FileStream stream;
		
		std::string vertex_shader_source = stream.ReadFile("resources/shaders/test_shader.vert").value();
		std::string fragment_shader_source = stream.ReadFile("resources/shaders/test_shader.frag").value();

		if (CHEMICAL_QUERY_ERROR) {
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

	void Renderer3D::RenderScene(const Scene& scene) {

		program->BindProgram();

		program->SetUniformMatrix4FV("projection", 1, false, &projection[0][0]);
		program->SetUniformMatrix4FV("view", 1, false, &glm::inverse(camera.UpdateMovement(wrapper))[0][0]);

		for(const std::shared_ptr<Mesh>& mesh : scene.meshes) {

			program->SetUniformMatrix4FV("model", 1, false, &mesh->transform.TransformToMat4()[0][0]);
			mesh->v_array.Bind();
			mesh->v_array.DrawElements(mesh->draw_info);
		}
		for (const Scene& inner_scene : scene.scenes) {
			for (const std::shared_ptr<Mesh>& mesh : inner_scene.meshes) {

				program->SetUniformMatrix4FV("model", 1, false, &mesh->transform.TransformToMat4()[0][0]);
				mesh->v_array.Bind();
				mesh->v_array.DrawElements(mesh->draw_info);
			}
		}

	}
}
