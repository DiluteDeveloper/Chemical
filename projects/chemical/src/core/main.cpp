#include <pch.h>

#include "engine.h"

int main(int argc, char* argv[]) {

	Chemical::ChemicalEngine engine;
	if (QUERY_ERROR) {
		CONSOLE_PRINT(Severity::_ERROR, "ChemicalEngine initialization failed.");
	}


	// GLAD PREFERENCES -----------------------------------------

	while (engine.is_running()) {

		engine.Update();
	}
}