#include "ControlsScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"
#include "KeyObserver.h"

#include <GLFW\glfw3.h>

ControlsScreen::ControlsScreen()
{
	auto renderSystem = std::make_unique<RenderSystem>(m_scene);

	// Create environment map / skybox
	Entity& skybox = Prefabs::createSkybox(m_scene, {
		"Assets/Textures/envmap_violentdays/violentdays_rt.tga",
		"Assets/Textures/envmap_violentdays/violentdays_lf.tga",
		"Assets/Textures/envmap_violentdays/violentdays_up.tga",
		"Assets/Textures/envmap_violentdays/violentdays_dn.tga",
		"Assets/Textures/envmap_violentdays/violentdays_bk.tga",
		"Assets/Textures/envmap_violentdays/violentdays_ft.tga",
		});
	Texture irradianceMap = GLUtils::loadCubeMapFaces({
		"Assets/Textures/envmap_violentdays/violentdays_irr_c00.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c01.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c02.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c03.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c04.bmp",
		"Assets/Textures/envmap_violentdays/violentdays_irr_c05.bmp",
		});
	Texture radianceMap = GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays_pmrem.dds");
	renderSystem->setRadianceMap(radianceMap.id);
	renderSystem->setIrradianceMap(irradianceMap.id);

	// Setup the camera
	Entity& cameraEntity = Prefabs::createCamera(m_scene, { 0, 23 * 8, 26 }, { 0, 0, 5 }, { 0, 1, 0 });
	renderSystem->setCamera(&cameraEntity);

	// Create the main menu quit button
	createTextLabel("Back", glm::vec2(112.0f, 112.0f), &m_UIButtons);

	// Create the main menu title text
	createTextLabel("Welcome to Battle Snake Racers!", glm::vec2(40.0f, 760.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu sub title text
	createTextLabel("Your goal is to fight to the death on the race course, the first to 10 points wins!", glm::vec2(40.0f, 700.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu title text
	createTextLabel("Earn points by being the last racer to survive each round!", glm::vec2(40.0f, 670.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu sub title text
	createTextLabel("Controls:", glm::vec2(40.0f, 610.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu title text
	createTextLabel("Left Analog: move    A : accelerate    X: brake", glm::vec2(40.0f, 580.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu sub title text
	createTextLabel("Collect pickups to grow your tail.", glm::vec2(40.0f, 520.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu title text
	createTextLabel("Avoid rival’s snake tails, falling off the track, and falling off the back of the camera.", glm::vec2(40.0f, 460.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	// Create the main menu sub title text
	createTextLabel("Good luck racers!", glm::vec2(40.0f, 400.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
}

ControlsScreen::~ControlsScreen()
{
}

void ControlsScreen::buttonPressed()
{
	// Back Pressed
	if (m_iActiveMenuButtonNumber == 0)
	{
		m_screenToTransitionTo = MAINMENU;
		m_bChangeScreen = true;
	}
}