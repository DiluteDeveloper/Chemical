#include "TimeLayer.h"

namespace Chemical {

	namespace Util {
		TimeLayer::TimeLayer(const Core::WindowLayer& w) : w(&w) {

			oldTime = w.GetTime();
		}

		void TimeLayer::Update() {
			double newTime = w->GetTime();
			deltaTime = newTime - oldTime;
			oldTime = newTime;
		}
	}

}
