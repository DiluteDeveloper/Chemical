#pragma once

#include <memory>
#include <functional>

namespace Chemical {

	namespace Core {

		class Time {
			friend class Application;

			double oldTime = 0;

			Time(const Time&) = delete;
			Time(Time&&) = delete;

			Time(double time);

			void Update(double time);

			double deltaTime = 0;
		};
	}

}