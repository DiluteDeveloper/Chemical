#pragma once

#include "core/application.h"
#include "world/scene.h"

class GameLayer : public Chemical::Core::ApplicationLayer {

private:
	using Chemical::Core::ApplicationLayer::ApplicationLayer;
	friend Chemical::Core::ApplicationData;

	void InitializeLayer() override;

	Chemical::Scene scene;
};
