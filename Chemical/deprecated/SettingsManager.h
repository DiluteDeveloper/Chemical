#pragma once

#include <vendor/json/json.hpp>
#include <vendor/glm/glm.hpp>

namespace Chemical {
	namespace Core {

		struct WindowSettings {

			bool vSync = false;
			bool fullscreen = false;

			glm::ivec2 resolution = glm::ivec2(1280, 720);

			std::string title = "Chemical";
		};
		class SettingsManager {
			friend class Application;

			SettingsManager(const SettingsManager&) = delete;
			SettingsManager(SettingsManager&&) = delete;

			nlohmann::json settings;

			~SettingsManager();
			SettingsManager();
		public:
			WindowSettings windowSettings;
		};

	}
}

