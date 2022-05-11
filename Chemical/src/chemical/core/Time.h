#pragma once

#include "AccessKey.h"
#include "Window.h"

#include <iostream>
#include <memory>
#include <functional>

namespace Chemical {

	namespace Core {

		class Application;
		class Time {

			const Window& window;

			double oldTime = 0;

			Time(const Time&) = delete;
			Time(Time&&) = delete;

		public:

			~Time() { std::cout << "Time destructor called" << std::endl; };
			Time(AccessKey<Application>, const Window& window);

			void Update(AccessKey<Application>);

			double deltaTime = 0;
		};
	}

}