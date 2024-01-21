#include "pch.h"

#include "engine.h"

using namespace Chemical::Core;
int main(int argc, char* argv[]) {

	Engine engine;


	while (!engine.windowSystem->shouldWindowClose()) {
		engine.Update();
	}

	return 0;
}