#include <pch.h>

#include "engine.h"

using namespace Chemical;

int main(int argc, char* argv[]) {

	ChemicalEngine engine;
	if (QUERY_ERROR) {
		CHEMICAL_PRINT(Severity::_ERROR, "ChemicalEngine initialization failed.");
	}


	// GLAD PREFERENCES -----------------------------------------

	while (engine.is_running()) {

		engine.Update();
	}
}