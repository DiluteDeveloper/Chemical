#pragma once

#include <memory>
#include <vector>
#include <string_view>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "transform/transform.h"
#include "graphics/opengl/texture.h"
#include "graphics/opengl/shader_program.h"
#include "graphics/renderer.h"

struct GLFWwindow;

namespace Chemical {

	class Core {
        public:
            std::unique_ptr<Graphics::Renderer> renderer;

            Transform player_transform;
            glm::mat4 projection;

            GLFWwindow* window = nullptr;

            Core();
            ~Core();
            void UpdatePlayer();
            void Update();
            void SetupImGui(const std::string_view& glsl_version);

            float camSpeed = 0.4f;
            float sensitivity = 0.8f;
            double oldx = 0, oldy = 0;
	
	};
}
