#pragma once

namespace Chemical {
	namespace GUI {

		// needs to be called before any other GUI functionality
		void InitializeGUI();

		void NewFrame(); // typically goes at start of engine loop
		void Render(); // typically goes at end of engine loop
	}
}
