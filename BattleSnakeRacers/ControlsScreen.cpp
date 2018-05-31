#include "ControlsScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"
#include "KeyObserver.h"
#include "Audio.h"

#include <GLFW\glfw3.h>

ControlsScreen::ControlsScreen()
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

	// Create the controls diagrams quad
	Entity& controlDiagrams = Prefabs::createQuad(m_scene, controlsTransform);
	controlDiagrams.transform.eulerAngles = { 1.5708f, 0, 0 };
	controlDiagrams.transform.scale = { 1.5, 1, 1 };
	controlDiagrams.transform.position = { 0, 180, 26 };
	controlDiagrams.model.materials.at(0).colorMaps.at(0) = GLUtils::loadTexture("Assets/Controls/controls.png"); \
		m_controlDiagram = &controlDiagrams;

	// Create the main menu quit button
	createTextLabel("Back", glm::vec2(112.0f, 112.0f), &m_UIButtons);

	// Create the main menu title text
	createTextLabel("Welcome to Battle Snake Racers!", glm::vec2(40.0f, 760.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Your goal is to fight to the death on the race course, the first to 10 points wins!", glm::vec2(40.0f, 700.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Earn points by being the last racer to survive each round!", glm::vec2(40.0f, 670.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Collect pickups to grow your tail.", glm::vec2(40.0f, 610.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Avoid rival’s snake tails, falling off the track, and falling off the back of the camera.", glm::vec2(40.0f, 580.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Good luck racers!", glm::vec2(40.0f, 520.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
	createTextLabel("Controls:", glm::vec2(40.0f, 460.0f), &m_UITexts, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
	m_screenState = CONTROLS;
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
	// Play a sound
	Audio& audio = Audio::getInstance();
	audio.playSFX(BUTTON_CLICKED);
}