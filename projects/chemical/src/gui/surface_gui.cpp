#include <pch.h>

#include <imgui.h>

#include "surface_gui.h"


namespace Chemical {
	namespace GUI {

		void SurfaceGUI::Update() {
			ImGui::Begin("Hello, world!");

			ImGui::Text("This is some useful text.");

			ImGui::End();
		}
	}
}