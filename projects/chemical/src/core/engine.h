#pragma once

#include "input.h"
#include "renderer.h"
#include "window_system.h"

#include <memory>

namespace Chemical {
	namespace Core {
		
		class Engine {
		public:

			std::unique_ptr<Input> input;
			std::unique_ptr<Renderer> renderer;
			std::unique_ptr<WindowSystem> windowSystem;

			Engine();
			~Engine();
			void Update();

		};
	
	}
}