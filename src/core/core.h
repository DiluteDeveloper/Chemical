#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "transform/transform.h"
#include "graphics/opengl/texture.h"
#include "graphics/opengl/shader_program.h"
#include "chunk_system.h"
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

            void UpdatePlayer();
            void Frame(); // one iteration of the main loop

            float camSpeed = 0.2f;
            float sensitivity = 0.08f;
            double oldx = 0, oldy = 0;
	
	};
}
