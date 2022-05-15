#include "SettingsManager.h"
#include "chemical/io/FileStream.h"

#include <fstream>
#include <iostream>

namespace Chemical {
	namespace Core {

		SettingsManager::SettingsManager() {

			try {
				settings = nlohmann::json::parse(IO::ReadFileAppData("Chemical/settings.json"));
			}
			catch (std::exception&) {
				std::cout << "\x1B[93msettings.json was not loaded properly. using settings defaults.\033[0m\t\t" << std::endl;
				return;

			}


			windowSettings.vSync = settings["window"]["v_sync"];
			windowSettings.resolution.x = settings["window"]["resolution"][0];
			windowSettings.resolution.y = settings["window"]["resolution"][1];
			windowSettings.title = settings["window"]["title"];
			windowSettings.fullscreen = settings["window"]["fullscreen"];
		}

		SettingsManager::~SettingsManager() {

			settings["window"]["v_sync"] = windowSettings.vSync;
			settings["window"]["resolution"][0] = windowSettings.resolution.x;
			settings["window"]["resolution"][1] = windowSettings.resolution.y;
			settings["window"]["title"] = windowSettings.title;
			settings["window"]["fullscreen"] = windowSettings.fullscreen;

			IO::WriteFileAppData("Chemical/settings.json", settings.dump());
		}
	}
}