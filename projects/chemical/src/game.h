#pragma once

#include "world/scene.h"

namespace Chemical {
	class ChemicalEngine;
}


class Game {
public:
	Game(Chemical::ChemicalEngine* engine);

	void Update(Chemical::ChemicalEngine* engine);

private:

	Chemical::Scene scene;
};
