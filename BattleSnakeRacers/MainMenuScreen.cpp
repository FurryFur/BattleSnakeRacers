#include "MainMenuScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"
#include "KeyObserver.h"

#include <GLFW\glfw3.h>

MainMenuScreen::MainMenuScreen()
{
	auto renderSystem = std::make_unique<RenderSystem>(m_scene);

	// Create environment map / skybox
	Entity& skybox = Prefabs::createSkybox(m_scene, GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays.dds"));
	Texture irradianceMap = GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays_iem.dds");
	Texture radianceMap = GLUtils::loadDDSTexture("Assets/Textures/envmap_violentdays/violentdays_pmrem.dds");
	renderSystem->setRadianceMap(radianceMap.id);
	renderSystem->setIrradianceMap(irradianceMap.id);

	// Setup the camera
	Entity& cameraEntity = Prefabs::createCamera(m_scene, { 0, 23 * 8, 26 }, { 0, 0, 5 }, { 0, 1, 0 });
	renderSystem->setCamera(&cameraEntity);

	// Create the main menu Start button
	createTextLabel("Start", glm::vec2(620.0f, 312.0f), &m_UIButtons);
	// Create the main menu controls button
	createTextLabel("Controls", glm::vec2(570.0f, 212.0f), &m_UIButtons);
	// Create the main menu quit button
	createTextLabel("Quit", glm::vec2(640.0f, 112.0f), &m_UIButtons);

	// Create the main menu title text
	createTextLabel("Battle Snake", glm::vec2(280.0f, 580.0f), &m_UITexts, 2.0f, glm::vec3(1.0f, 1.0f, 1.0));
	// Create the main menu sub title text
	createTextLabel("Racers!", glm::vec2(700.0f, 480.0f), &m_UITexts, 2.0f, glm::vec3(1.0f, 1.0f, 1.0));
	// Create the main menu credits text
	createTextLabel("Made by: Lance Chaney, Jack Mair, Hugo Adams, Blair Corban", glm::vec2(400.0f, 10.0f), &m_UITexts, 0.5f, glm::vec3(1.0f, 1.0f, 1.0));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
	m_screenState = MAINMENU;
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::buttonPressed()
{
	// Start Pressed
	if (m_iActiveMenuButtonNumber == 0)
	{
		//m_screenToTransitionTo = GAMEPLAY;
		m_screenToTransitionTo = PLAYERSELECT;
		m_bChangeScreen = true;
	}
	// Controls Pressed
	else if (m_iActiveMenuButtonNumber == 1)
	{
		m_screenToTransitionTo = CONTROLS;
		m_bChangeScreen = true;
	}
	// Quit Pressed
	else if (m_iActiveMenuButtonNumber == 2)
	{

	}
}