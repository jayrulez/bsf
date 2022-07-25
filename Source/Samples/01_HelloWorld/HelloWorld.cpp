//#include "BsEntry.h"
#include "Foundation/bsfEngine/BsEntry.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCCamera.h"

using namespace bs;

UINT32 windowResWidth = 1280;
UINT32 windowResHeight = 720;

int bs_main(int argc, char* argv[])
{
	// Define a video mode for the resolution of the primary rendering window.
	VideoMode videoMode(windowResWidth, windowResHeight);

	Application::startUp(videoMode, "01 - Hello World", false);

	HSceneObject sceneCameraSO = SceneObject::create("SceneCamera");
	HCamera sceneCamera = sceneCameraSO->addComponent<CCamera>();
	sceneCamera->setMain(true);

	sceneCameraSO->setPosition(Vector3(40.0f, 30.0f, 230.0f));
	sceneCameraSO->lookAt(Vector3(0, 0, 0));

	// Runs the main loop that does most of the work. This method will exit when user closes the main
	// window or exits in some other way.
	Application::instance().runMainLoop();

	// Clean up when done
	Application::shutDown();

	return 0;
}

