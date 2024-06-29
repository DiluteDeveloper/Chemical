#include <pch.h>

#include "sprite_renderer.h"

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"
#include "util/filestream.h"

#include "core.h"

namespace Chemical {
	namespace Core {


		std::unique_ptr<OpenGL::VertexArray> v_array; 
		OpenGL::ElementDrawInfo info;
		OpenGL::VertexLayout layout;
		std::unique_ptr<OpenGL::ShaderProgram> shader_program;

		void SpriteRenderer::InitializeSpriteRenderer() {
			OpenGL::Buffer v_buffer;

			v_array = std::make_unique<OpenGL::VertexArray>();

			float vertices[] = {
				-0.5f, -0.5f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.0f, 1.0f,
				0.5f, 0.5f, 1.0f, 1.0f,
				0.5f, -0.5f, 1.0f, 0.0f
			};
			v_buffer.CreateImmutableBuffer(sizeof(float) * 16, &vertices[0]);
			unsigned int indices[] = {
				2, 1, 0,
				2, 0, 3
			};
			OpenGL::Buffer e_buffer;
			e_buffer.CreateImmutableBuffer(sizeof(unsigned int) * 6, &indices[0]);

			layout.AddAttribute(OpenGL::VertexAttribute(2, 0, OpenGL::DataType::FLOAT));
			layout.AddAttribute(OpenGL::VertexAttribute(2, sizeof(float) * 2, OpenGL::DataType::FLOAT));
			info.count = 6;
			info.data_type = OpenGL::DataType::UNSIGNED_INT;
			info.mode = OpenGL::DrawMode::TRIANGLES;
			info.offset = 0;

			v_array->SetVertexBuffer(v_buffer, layout, 0, 0);
			v_array->SetElementBuffer(e_buffer);

			OpenGL::Shader v_shader(Util::ReadFile("resources/shaders/sprite_2D.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
			OpenGL::Shader f_shader(Util::ReadFile("resources/shaders/sprite_2D.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

			shader_program = std::make_unique<OpenGL::ShaderProgram>(OpenGL::ShaderProgram{ { &v_shader, &f_shader }, layout });

			// screen dimensions
			glm::mat4 projection_matrix = glm::ortho(-Core::window_size.x / 2.0f, Core::window_size.x / 2.0f,
				-Core::window_size.y / 2.0f, Core::window_size.y / 2.0f);
			shader_program->SetUniformMatrix4FV("projection_matrix", 1, false, &projection_matrix[0][0]);
			glm::mat4 view_matrix = glm::mat4(1.0f);
			shader_program->SetUniformMatrix4FV("view_matrix", 1, false, &view_matrix[0][0]);
		}
		void SpriteRenderer::RenderSprites(const std::vector<Node::Sprite2D>& sprites) {
			shader_program->BindProgram();

			for (auto& sprite : sprites) {
				glm::mat4 real = sprite.node_2d.model_matrix;
				real = glm::scale(real, glm::vec3(sprite.image_scale, 1));
				shader_program->SetUniformMatrix4FV("model_matrix", 1, false, &real[0][0]);
				sprite.image_texture->BindTexture(0);

				v_array->Bind();
				v_array->DrawElements(info);
			}
		}
	}
}