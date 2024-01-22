#pragma once

#include "Input.h"
#include "rendering/Renderer3D.h"
#include "graphics/opengl/Handler.h"
#include "WindowSystem.h"

#include "world/Scene.h"

#include <memory>

namespace Chemical {
	namespace Core {
		
		class Engine {
		public:

			Engine();
			~Engine();
			void Update();

			// editor scene
			std::unique_ptr<World::Scene> loadedEditorScene;
			std::unique_ptr<Input> input;

			std::unique_ptr<Rendering::Renderer3D> renderer;

			// may need to make multiple so that rendering opengl code doesnt happen at teh same time as mesh creation
			std::unique_ptr<OpenGL::Handler> glHandler;

			std::unique_ptr<WindowSystem> windowSystem;

		};
	
	}
}