#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer_layer.h"
#include "scene_layer.h"
#include "rendering/opengl/shader_program.h"
#include "util/filestream.h"
#include "core/glfw_wrapper.h"

#include "scene_objects/mesh_3d.h"


namespace Chemical {

	namespace Core {
		using namespace Scene;

		void RendererLayer::InitializeLayer() {
			Mesh3D::Vertex::InitializeVertexLayout();

			Util::FileStream stream;

			std::string vertex_shader_source = stream.ReadFile("resources/shaders/test_shader.vert").value();
			std::string fragment_shader_source = stream.ReadFile("resources/shaders/test_shader.frag").value();

			OpenGL::Shader vertex_shader(vertex_shader_source.c_str(), OpenGL::ShaderType::VERTEX_SHADER);
			OpenGL::Shader fragment_shader(fragment_shader_source.c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

			program = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
				const OpenGL::Shader*>{ &vertex_shader, & fragment_shader }, Mesh3D::Vertex::vertex_layout);

			// perspectiveLH converts left handed input data to opengls coordinate system
			projection = glm::perspectiveLH(glm::radians(90.0f), m_app_data.GetGLFWWrapper().GetWindowSizeX() / 
				static_cast<float>(m_app_data.GetGLFWWrapper().GetWindowSizeY()), 0.1f, 1000.0f);
		}

		void RendererLayer::UpdateLayer() {
			SceneLayer* scene_layer = m_app_data.GetLayer<SceneLayer>("SceneLayer");
			if (scene_layer == nullptr)
				return;
			const Object* root = scene_layer->GetRoot();
			if (root == nullptr)
				return;
			if (root->GetSuperiorDescriptor() != "Mesh3D")
				return;
			const Mesh3D& root_mesh = root->GetSuperior<Mesh3D>();
			Camera3D* camera = scene_layer->GetCamera3D();
			if (camera == nullptr)
				return;

			program->BindProgram();

			program->SetUniformMatrix4FV("projection", 1, false, &projection[0][0]);
			program->SetUniformMatrix4FV("view", 1, false, &glm::inverse(camera->UpdateMovement())[0][0]);

			program->SetUniformMatrix4FV("model", 1, false, &root_mesh.object_3d.transform.TransformToMat4()[0][0]);
			root_mesh.v_array.Bind();
			root_mesh.v_array.DrawElements(root_mesh.draw_info);

			program->SetUniformMatrix4FV("model", 1, false, &root_mesh.object_3d.transform.TransformToMat4()[0][0]);
			root_mesh.v_array.Bind();
			root_mesh.v_array.DrawElements(root_mesh.draw_info);
			/*for (const std::shared_ptr<Mesh>& mesh : scene->meshes) {

				program->SetUniformMatrix4FV("model", 1, false, &mesh->transform.TransformToMat4()[0][0]);
				mesh->v_array.Bind();
				mesh->v_array.DrawElements(mesh->draw_info);
			}
			for (const Scene& inner_scene : scene->scenes) {
				for (const std::shared_ptr<Mesh>& mesh : inner_scene.meshes) {

					program->SetUniformMatrix4FV("model", 1, false, &mesh->transform.TransformToMat4()[0][0]);
					mesh->v_array.Bind();
					mesh->v_array.DrawElements(mesh->draw_info);
				}
			}*/

		}
	}


}
