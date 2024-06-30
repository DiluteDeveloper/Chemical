#pragma once

namespace Chemical {
	namespace GUI {

		// Requires Chemical::Core::Window
		void InitializeGUI();

		void NewFrame(); // typically goes at start of engine loop
		void Render(); // typically goes at end of engine loop
	}
}
