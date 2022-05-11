#include "Application.h"

int main() {
	Chemical::Core::Application* app = Chemical::Core::CreateApplication();
	app->Run();
	delete app;
	return 0;
}