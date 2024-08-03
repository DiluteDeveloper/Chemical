#include <pch.h>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer_layer.h"
#include "scene_layer.h"
#include "rendering/opengl/shader_program.h"
#include "util/filestream.h"
#include "core/glfw_wrapper.h"


namespace Chemical {

	namespace Core {

		void RendererLayer::InitializeLayer(ApplicationData& appdata) {

			m_sceneLayer = appdata.GetLayer<SceneLayer>("SceneLayer");
			if (m_sceneLayer == nullptr)
				throw std::exception();


			GeneratedMesh3D::Vertex::InitializeVertexLayout();

			Util::FileStream stream;

			std::string vertexShaderSource = stream.ReadFile("resources/shaders/test_shader.vert").value();
			std::string fragmentShaderSource = stream.ReadFile("resources/shaders/test_shader.frag").value();

			OpenGL::Shader vertexShader(vertexShaderSource.c_str(), OpenGL::ShaderType::VERTEX_SHADER);
			OpenGL::Shader fragmentShader(fragmentShaderSource.c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

			m_program = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
				const OpenGL::Shader*>{ &vertexShader, & fragmentShader }, GeneratedMesh3D::Vertex::vertexLayout);

			// perspectiveLH converts left handed input data to opengls coordinate system
			m_projection = glm::perspectiveLH(glm::radians(90.0f), appdata.GetGLFWWrapper().GetWindowSizeX() / 
				static_cast<float>(appdata.GetGLFWWrapper().GetWindowSizeY()), 0.1f, 1000.0f);
		}

		void RendererLayer::UpdateLayer() {
			
			const Camera3D& camera = m_sceneLayer->scene->GetCamera3D();
			const Mesh3D& mesh = m_sceneLayer->scene->GetMeshes3D()[0];

			m_program->BindProgram();

			m_program->SetUniformMatrix4FV("projection", 1, false, &m_projection[0][0]);
			m_program->SetUniformMatrix4FV("view", 1, false, &glm::inverse(camera.GetView())[0][0]);

			m_program->SetUniformMatrix4FV("model", 1, false, &mesh.transform.TransformToMat4()[0][0]);
			mesh.genMesh.vArray.Bind();
			mesh.genMesh.vArray.DrawElements(mesh.genMesh.drawInfo);

			m_program->SetUniformMatrix4FV("model", 1, false, &mesh.transform.TransformToMat4()[0][0]);
			mesh.genMesh.vArray.Bind();
			mesh.genMesh.vArray.DrawElements(mesh.genMesh.drawInfo);
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
