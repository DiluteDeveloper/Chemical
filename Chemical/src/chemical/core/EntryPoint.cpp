#include "Application.h"
#include "chemical/io/ConsoleStream.h"

int main() {
	Chemical::Core::Application* app = Chemical::Core::CreateApplication();

	Chemical::IO::LogMessage("Chemical v1.1 initialized.");
	app->Run();
	delete app;
	return 0;
}