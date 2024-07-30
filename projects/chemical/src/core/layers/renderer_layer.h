#pragma once

#include "core/application.h"
#include "core/glfw_wrapper.h"
#include "rendering/opengl/shader_program.h"
#include "rendering/camera.h"
#include "world/scene.h"

namespace Chemical {
	
	namespace Core {

		class RendererLayer : protected ApplicationLayer {
		private:
			using ApplicationLayer::ApplicationLayer;
			friend ApplicationData;

			void InitializeLayer() override;
			void UpdateLayer() override;

			std::unique_ptr<Camera> camera;

			glm::mat4 view = glm::mat4(1.0f);
			OpenGL::VertexLayout layout;

			glm::mat4 projection = glm::mat4(1.0f);

			std::unique_ptr<OpenGL::ShaderProgram> program;
		};
	}

}
