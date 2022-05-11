#pragma once

#include "LayerStack.h"
#include "AssetImporter.h"
#include "Window.h"
#include "Time.h"

#include <memory>

int main();

namespace Chemical {

	namespace Core {

		class Application {

			std::unique_ptr<Window> window;
			std::unique_ptr<AssetImporter> assetImporter;
			std::unique_ptr<Time> time;
			std::unique_ptr<LayerStack> layerStack;


			friend int ::main();
			void Run();

			Application(const Application&) = delete;
			Application(Application&&) = delete;

		protected:
			Application();

			inline virtual ~Application() {
				std::cout << "Application destructor called." << std::endl;
			};

		public:
			Window& GetWindow() { return *window.get(); };
			AssetImporter& GetAssetImporter() { return *assetImporter.get(); };
			Time& GetTime() { return *time.get(); };
		protected:
			LayerStack& GetLayerStack() { return *layerStack.get(); };
		};

		extern Application* CreateApplication();
	}



}
