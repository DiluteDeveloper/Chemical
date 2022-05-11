#include "Time.h"
#include "Application.h"

namespace Chemical {

	namespace Core {
		Time::Time(AccessKey<Application>, const Window& window) : window(window) {
			std::cout << "Time constructor called." << std::endl;

			oldTime = window.GetTime();
		}

		void Time::Update(AccessKey<Application>) {
			double newTime = window.GetTime();
			deltaTime = newTime - oldTime;
			oldTime = newTime;
		}
	}

}
