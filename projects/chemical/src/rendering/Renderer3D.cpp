#include "pch.h"

#include "Renderer3D.h"

#include "util/Filestream.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

namespace Chemical {
	namespace Rendering {

		Renderer3D::Renderer3D(GLFWwindow* window, OpenGL::Handler* h, glm::fvec3 clearColour) : window(window), handler(h) {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
#ifdef CHEMICAL_DEBUG
				LOGGER_CONSOLE_ERROR("gladLoadGL failed.");
#endif
				throw std::exception();
			}
#ifdef CHEMICAL_DEBUG
			else
				LOGGER_CONSOLE_MESSAGE("gladLoadGL succeeded.");
#endif

			glClearColor(clearColour.r, clearColour.y, clearColour.b, 1.0f);

			OpenGL::VertexAttribute att(3, 0);
			OpenGL::VertexAttribute att2(3, sizeof(float) * 3);
			defLayout.AddAttribute(att);
			defLayout.AddAttribute(att2);

			OpenGL::Shader vertex_chunk_shader(Util::ReadFile("resources/shaders/default_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
			OpenGL::Shader fragment_chunk_shader(Util::ReadFile("resources/shaders/default_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);

			OpenGL::VertexLayout vertex_layout;
			vertex_layout.AddAttribute(OpenGL::VertexAttribute{1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT});

			program = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
				const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader }, vertex_layout);

		}

		void Renderer3D::Update() {

			program->BindProgram();

			for (const RawMesh3D& r : rawMeshes) {
				
				handler->SubmitFunctionExecution([&r] {
					r.vArray.Bind();

					r.vArray.DrawElements(r.info);
					});



			}

			glfwSwapBuffers(window);
		}

		RawMesh3D* Renderer3D::CreateRawMesh(const OpenGL::ElementDrawInfo& info) {
			return &rawMeshes.emplace_back(info);
		}
	}
}
