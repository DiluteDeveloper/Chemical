#pragma once

#include "core/application.h"
#include "core/layers/scene_layer.h"
#include "rendering/opengl/shader_program.h"

namespace Chemical {
	
	namespace Core {

		class RendererLayer : public ApplicationLayer {
		private:

			// only valid after initialization
			SceneLayer* m_sceneLayer = nullptr;

			void InitializeLayer(ApplicationData& appData) override;
			void UpdateLayer() override;

			glm::mat4 m_projection = glm::mat4(1.0f);

			std::unique_ptr<OpenGL::ShaderProgram> m_program;
		};
	}

}
