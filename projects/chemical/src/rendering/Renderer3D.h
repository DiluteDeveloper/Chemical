#pragma once

struct GLFWwindow;

#include <glm/glm.hpp>

#include "RawMesh3D.h"

#include "graphics/opengl/Handler.h"

#include "graphics/opengl/ShaderProgram.h"

namespace Chemical {
	namespace Rendering {

		class Renderer3D {
		public:

			// called by engine only
			Renderer3D(GLFWwindow* window, OpenGL::Handler* h, glm::fvec3 clearColour = glm::fvec3(1.0f));

			// called by engine only
			void Update();

			// renderer3d owns this mesh, not you
			RawMesh3D* CreateRawMesh(const OpenGL::ElementDrawInfo& info);

			OpenGL::VertexLayout defLayout;

		private:

			std::unique_ptr<OpenGL::ShaderProgram> program;
			GLFWwindow* window;

			OpenGL::Handler* handler;

			std::vector<RawMesh3D> rawMeshes;
		};
	}
}
