#pragma once

namespace Chemical {

	namespace Core {
		class Window;
		class SettingsManager;
		class Application;
	}
	namespace GUI {

		class GUIManager {
			friend Core::Application;

			GUIManager(const GUIManager&) = delete;
			GUIManager(GUIManager&&) = delete;

			GUIManager() = default;
			~GUIManager() = default;

			void Update(Core::SettingsManager& sm, Core::Window& w);
		};
	}
}