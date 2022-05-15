#pragma once
int main();

namespace Chemical {

	namespace Core {

		class Window;
		class AssetImporter;
		class Time;
		class LayerStack;

		class Application {

			Window* window;
			AssetImporter* assetImporter;
			Time* time;


			friend int ::main();
			void Run();

			Application(const Application&) = delete;
			Application(Application&&) = delete;

		protected:
			LayerStack* layerStack;


			Application();

			virtual ~Application();
		};

		extern Application* CreateApplication();
	}


}
