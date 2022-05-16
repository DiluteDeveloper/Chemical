#pragma once

#include "chemical/core/Layer.h"
#include "chemical/core/layers/WindowLayer.h"
#include "chemical/core/LayerStack.h"

namespace Chemical {

	namespace Util {

		class TimeLayer : public Core::Layer {

			friend class Core::LayerStack;

			double oldTime = 0;

			const Core::WindowLayer* w;

			TimeLayer(const TimeLayer&) = delete;
			TimeLayer(TimeLayer&&) = delete;
			TimeLayer(const Core::WindowLayer& w);
			~TimeLayer() override = default;

		public:

			void Update();

			double deltaTime = 0;
		};
	}

}