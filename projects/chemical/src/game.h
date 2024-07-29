#pragma once

#include "world/scene.h"

namespace Chemical {
	class Application;
}


class Game {
public:
	Game(Chemical::Application* engine);

	void Update(Chemical::Application* engine);

private:

	Chemical::Scene scene;
};
