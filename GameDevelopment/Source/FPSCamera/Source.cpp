// Framework includes
#include "BsApplication.h"
#include "Resources/BsResources.h"
#include "Resources/BsBuiltinResources.h"
#include "Material/BsMaterial.h"
#include "Components/BsCCamera.h"
#include "Components/BsCRenderable.h"
#include "Components/BsCSkybox.h"
#include "Components/BsCPlaneCollider.h"
#include "Components/BsCBoxCollider.h"
#include "Components/BsCCharacterController.h"
#include "Components/BsCRigidbody.h"
#include "GUI/BsCGUIWidget.h"
#include "GUI/BsGUIPanel.h"
#include "GUI/BsGUILayoutY.h"
#include "GUI/BsGUILabel.h"
#include "Physics/BsPhysicsMaterial.h"
#include "RenderAPI/BsRenderAPI.h"
#include "RenderAPI/BsRenderWindow.h"
#include "Scene/BsSceneObject.h"
#include "Platform/BsCursor.h"
#include "Input/BsInput.h"
#include "BsExampleFramework.h"
#include "FPSCamera.h"


namespace bs
{
	constexpr float GROUND_PLANE_SCALE = 50.0f;

	UINT32 windowResWidth = 1280;
	UINT32 windowResHeight = 720;

	void setUpScene()
	{
		HTexture gridPattern = ExampleFramework::loadTexture(ExampleTexture::GridPattern);
		HTexture gridPattern2 = ExampleFramework::loadTexture(ExampleTexture::GridPattern2);

		HShader shader = gBuiltinResources().getBuiltinShader(BuiltinShader::Standard);

		HMaterial planeMaterial = Material::create(shader);
		planeMaterial->setTexture("gAlbedoTex", gridPattern2);

		planeMaterial->setVec2("gUVTile", Vector2::ONE * GROUND_PLANE_SCALE * 0.5f);

		HMaterial boxMaterial = Material::create(shader);
		boxMaterial->setTexture("gAlbedoTex", gridPattern);

		HMaterial sphereMaterial = Material::create(shader);

		HMesh boxMesh = gBuiltinResources().getMesh(BuiltinMesh::Box);
		HMesh planeMesh = gBuiltinResources().getMesh(BuiltinMesh::Quad);
		HMesh sphereMesh = gBuiltinResources().getMesh(BuiltinMesh::Sphere);

		HPhysicsMaterial boxPhysicsMaterial = PhysicsMaterial::create(1.0f, 1.0f, 0.0f);
		HPhysicsMaterial spherePhysicsMaterial = PhysicsMaterial::create(1.0f, 1.0f, 0.5f);


		/* 									FLOOR	                    		*/
		HSceneObject floorSO = SceneObject::create("Floor");
		HRenderable floorRenderable = floorSO->addComponent<CRenderable>();
		floorRenderable->setMesh(planeMesh);
		floorRenderable->setMaterial(planeMaterial);

		floorSO->setScale(Vector3(GROUND_PLANE_SCALE, 1.0f, GROUND_PLANE_SCALE));

		HPlaneCollider planeCollider = floorSO->addComponent<CPlaneCollider>();
		planeCollider->setMaterial(boxPhysicsMaterial);

		/* 									FPS_CAMERA                    		*/

		HSceneObject fpsCamera = SceneObject::create("FPSCamera");
		fpsCamera->setPosition(Vector3(0.0f, 1.0f, 5.0f));
		HCharacterController charController = fpsCamera->addComponent<CCharacterController>();

		charController->setHeight(1.0f);
		charController->setRadius(0.4f);

		fpsCamera->addComponent<gameDevelopnemt::FPSCamera>();


		SPtr<RenderWindow> window = gApplication().getPrimaryWindow();
		HCamera sceneCamera = fpsCamera->addComponent<CCamera>();
		sceneCamera->getViewport()->setTarget(window);
		sceneCamera->setNearClipDistance(0.005f);
		sceneCamera->setFarClipDistance(1000);
		sceneCamera->setAspectRatio(windowResWidth / (float)windowResHeight);

		gameDevelopnemt::HFPSCamera camera = fpsCamera->addComponent<gameDevelopnemt::FPSCamera>();
		camera->SetSceneObject(fpsCamera);
		fpsCamera->setPosition(Vector3(0.0f, 1.8f * 0.5f - 0.1f, 0.0f));

		/* 									SKYBOX                       		*/
		HTexture skyCubemap = ExampleFramework::loadTexture(ExampleTexture::EnvironmentDaytime, false, true, true);
		HSceneObject skyboxSO = SceneObject::create("Skybox");
		HSkybox skybox = skyboxSO->addComponent<CSkybox>();
		skybox->setTexture(skyCubemap);

		/* 									CURSOR                       		*/
		Cursor::instance().hide();
		Cursor::instance().clipToWindow(*window);

		// INPUT
		gInput().onButtonUp.connect([=](const ButtonEvent& ev)
		{
			if (ev.buttonCode == BC_ESCAPE)
			{
				gApplication().quitRequested();
			}
		});
		// GUI
		HSceneObject guiSO = SceneObject::create("GUI");
		HGUIWidget gui = guiSO->addComponent<CGUIWidget>(sceneCamera);

		GUIPanel* mainPanel = gui->getPanel();
		GUILayoutY* vertLayout = GUILayoutY::create();

		HString quitString(u8"Press the Escape key to quit");

		vertLayout->addNewElement<GUILabel>(quitString);
		mainPanel->addElement(vertLayout);
	}
}


#if BS_PLATFORM == BS_PLATFORM_WIN32
#include <windows.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
#else
int main()
#endif
{
	using namespace bs;

	VideoMode videoMode(windowResWidth, windowResHeight);
	Application::startUp(videoMode, "FPS CAMERA", false);
	ExampleFramework::setupInputConfig();

	setUpScene();

	Application::instance().runMainLoop();
	Application::shutDown();

	return 0;
}
