#pragma once

#include "core/application.h"

class GameLayer : public Chemical::Core::ApplicationLayer {

private:

	void InitializeLayer(Chemical::Core::ApplicationData& appData) override;
};
