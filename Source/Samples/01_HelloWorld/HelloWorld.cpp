//#include "BsEntry.h"
#include "Foundation/bsfEngine/BsEntry.h"

int bs_main(int argc, char* argv[])
{
	using namespace bs;

	VideoMode videoMode(1280, 720);
	Application::startUp(videoMode, "01 - Hello World", false);
	Application::instance().runMainLoop();
	Application::shutDown();

	return 0;
}
