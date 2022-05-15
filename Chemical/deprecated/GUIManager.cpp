#include "GUIManager.h"
#include "chemical/core/Window.h"

#include <vendor/imgui/imgui.h>

namespace Chemical {
	namespace GUI {

		void GUIManager::Update(Core::SettingsManager& sm, Core::Window& w) {

			ImGui::Begin("Navbar");
			if(ImGui::BeginCombo("##settings", "Settings")) {

				if (ImGui::BeginCombo("##settings_vsync", "VSync")) {

					if (ImGui::Button("On##settings_vsync_on")) {
						w.SetVSync(true);
						sm.windowSettings.vSync = true;
					}
					if (ImGui::Button("Off##settings_vsync_off")) {
						w.SetVSync(false);
						sm.windowSettings.vSync = false;
					}
					ImGui::EndCombo();
				}


				ImGui::EndCombo();
			}
			ImGui::End();
		}
	}
}