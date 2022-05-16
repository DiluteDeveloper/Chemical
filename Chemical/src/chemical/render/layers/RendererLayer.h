#pragma once

#include "chemical/core/Layer.h"
#include "chemical/core/layers/AssetImporterLayer.h"

namespace Chemical {
	namespace Render {

		class RendererLayer : public Core::Layer {

			Core::AssetImporterLayer* ai;

			void OnUpdate() override;

		public:

			RendererLayer(Core::AssetImporterLayer& ai) : ai(&ai) {};

			~RendererLayer() override = default;
		};
	}
}
