#pragma once

#include "core/engine.h"

#include "world/scene.h"

class Game {
public:
	Game(Chemical::ChemicalEngine& engine);

	void Update(Chemical::ChemicalEngine& engine);

private:

	Chemical::Scene scene;
};
