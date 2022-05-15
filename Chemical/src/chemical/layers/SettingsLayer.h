#pragma once

#include "Layer.h"
#include "chemical/core/Window.h"
#include "chemical/io/FileStream.h"
#include "chemical/io/ConsoleStream.h"

#include <vendor/json/json.hpp>
#include <vendor/glm/glm.hpp>

namespace Chemical {
	namespace Layers {
		class SettingsLayer : public Layer {
		public:

			SettingsLayer(const SettingsLayer&) = delete;
			SettingsLayer(SettingsLayer&&) = delete;

			nlohmann::json settings;

		public:

			Core::WindowSettings windowSettings;
			SettingsLayer() {
				try {
					settings = nlohmann::json::parse(IO::ReadFileAppData("Chemical/settings.json"));
				}
				catch (std::exception&) {
					IO::LogWarning("settings.json was not loaded properly. using settings defaults.");
					return;

				}


				windowSettings.vSync = settings["window"]["v_sync"];
				windowSettings.resolution.x = settings["window"]["resolution"][0];
				windowSettings.resolution.y = settings["window"]["resolution"][1];
				windowSettings.title = settings["window"]["title"];
				windowSettings.fullscreen = settings["window"]["fullscreen"];
			}

			~SettingsLayer() override {
				settings["window"]["v_sync"] = windowSettings.vSync;
				settings["window"]["resolution"][0] = windowSettings.resolution.x;
				settings["window"]["resolution"][1] = windowSettings.resolution.y;
				settings["window"]["title"] = windowSettings.title;
				settings["window"]["fullscreen"] = windowSettings.fullscreen;

				IO::WriteFileAppData("Chemical/settings.json", settings.dump());
			}
		};

	}
}

