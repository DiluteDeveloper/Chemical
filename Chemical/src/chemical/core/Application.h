#pragma once
int main();

#include "chemical/core/layers/SettingsLayer.h"
#include "chemical/core/layers/WindowLayer.h"
#include "chemical/core/layers/AssetImporterLayer.h"
#include "chemical/util/layers/TimeLayer.h"
#include "chemical/render/layers/RendererLayer.h"

#include "LayerStack.h"

namespace Chemical {

	namespace Core {

		class Application {
		protected:

			LayerStack& GetLayerStack();
			Util::TimeLayer& GetTime();

			Application();

			virtual ~Application() = default; // maybe std::unique_ptr in future

		private:
			WindowLayer* w = nullptr;
			Util::TimeLayer* t = nullptr;
			LayerStack* layerStack; // maybe std::unique_ptr in future

			friend int ::main();
			void Run();

			Application(const Application&) = delete;
			Application(Application&&) = delete;

		};

		extern Application* CreateApplication();
	}


}
