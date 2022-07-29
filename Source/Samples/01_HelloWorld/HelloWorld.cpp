//#include "BsEntry.h"
#include "Foundation/bsfEngine/BsEntry.h"
#include "Scene/BsSceneObject.h"
#include "Components/BsCCamera.h"
#include "Components/BsCRenderable.h"

#include "Resources/BsBuiltinResources.h"
#include "Resources/BsBuiltinResourcesHelper.h"

#include "Material/BsMaterial.h"

using namespace bs;

uint32_t windowResWidth = 1280;
uint32_t windowResHeight = 720;

int bs_main(int argc, char* argv[])
{
    // Define a video mode for the resolution of the primary rendering window.
    VideoMode videoMode(windowResWidth, windowResHeight);

    Application::startUp(videoMode, "01 - Hello World", false);

	HSceneObject sceneCameraSO = SceneObject::create("SceneCamera");
	HCamera sceneCamera = sceneCameraSO->addComponent<CCamera>();
	sceneCamera->setMain(true);
	sceneCameraSO->setPosition(Vector3(5.0f, 2.0f, 2.0f));
	sceneCameraSO->lookAt(Vector3(0, 0, 0));

	// Create a standard PBR material
	HShader shader = gBuiltinResources().getBuiltinShader(BuiltinShader::Standard);
	HMaterial material = Material::create(shader);

	HTexture texture = gBuiltinResources().getTexture(BuiltinTexture::White);
	material->setTexture("gAlbedoTex", texture);

	HMesh mesh = gBuiltinResources().getMesh(BuiltinMesh::Box);

	// Create a renderable
	HSceneObject renderableSO = SceneObject::create("3D object");
	HRenderable renderable = renderableSO->addComponent<CRenderable>();

	// Assign material and mesh to the renderable
	renderable->setMesh(mesh);
	renderable->setMaterial(material);

	// Optionally position the renderable in the scene
	renderableSO->setPosition(Vector3(20.0f, 0.0f, 15.0f));

	// profiley
	//gApplication().showProfilerOverlay(ProfilerOverlayType::CPUSamples);

    // Runs the main loop that does most of the work. This method will exit when user closes the main
    // window or exits in some other way.
    Application::instance().runMainLoop();

    // Clean up when done
    Application::shutDown();

    return 0;
}

