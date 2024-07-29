#pragma once

namespace Chemical {

	class Application;

	class GUI {
	public:
		GUI(Application* engine);

		void Update(Application* engine);
	};
}
