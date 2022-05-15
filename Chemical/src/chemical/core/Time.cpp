#include "Time.h"
#include "Application.h"

namespace Chemical {

	namespace Core {
		Time::Time(double time) {

			oldTime = time;
		}

		void Time::Update(double time) {
			double newTime = time;
			deltaTime = newTime - oldTime;
			oldTime = newTime;
		}
	}

}
