#pragma once

#include <memory>
#include <vector>
#include <string_view>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "transform/transform.h"
#include "graphics/opengl/texture.h"
#include "graphics/opengl/shader_program.h"
#include "world/chunk/loader.h"

struct GLFWwindow;

namespace Chemical {

	class Core {
        public:
            Transform player_transform;
            glm::mat4 projection;

            GLFWwindow* window = nullptr;

            // Everything the main loop touches lives here, so it stays alive after main()'s setup code.
            // The textures are only bound by texture unit, but they must not be destroyed (glDeleteTextures)
            // while the loop is running.
            std::vector<OpenGL::Texture> textures;
            std::unique_ptr<ChunkLoader> loader;
            std::unique_ptr<OpenGL::ShaderProgram> chunk_shader;

            Core();
            ~Core();
            void UpdatePlayer();
            void Update(); // one iteration of the main loop
            void SetupImGui(const std::string_view& glsl_version);

            float camSpeed = 0.2f;
            float sensitivity = 0.01f;
            double oldx = 0, oldy = 0;
	
	};
}
