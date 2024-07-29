#include <pch.h>

#include "application.h"

using namespace Chemical::Core;

int main(int argc, char* argv[]) {

	Application app;

	while (app.is_running()) {

		app.Update();
	}
}