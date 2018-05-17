#include "EndScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "Utils.h"
#include "CameraSystem.h"
#include "GLUtils.h"
#include "TextLabel.h"
#include "Audio.h"

using namespace std;
EndScreen::EndScreen(std::string _dataIn)
{

	string p1 = _dataIn.substr(0, 3);
	string p2 = _dataIn.substr(3, 3);
	string p3 = _dataIn.substr(6, 3);
	string p4 = _dataIn.substr(9, 3);

	int index = 0;
	for (int i = 0; i < _dataIn.length(); i++)
	{
		if (_dataIn[i] == 'W')
		{
			index = i;
			break;
		}
	}
	char pWin = '0' + ((index + 1) / 3);
	std::string winMsg = "Player ";
	winMsg += pWin;
	winMsg += " Wins!";
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

	createTextLabel(winMsg, glm::vec2(400, 700), &m_UITexts, 2.0f, glm::vec3(1, 1, 1));

	
	if (p1[0] != 'N')
	{
		createTextLabel("P1", glm::vec2(320, 500), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p1.substr(1,2), glm::vec2(320, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));		
		createTextLabel(p1.substr(0, 1), glm::vec2(320, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p2[0] != 'N')
	{
		createTextLabel("P2", glm::vec2(520, 500), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p2.substr(1, 2), glm::vec2(520, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p2.substr(0, 1), glm::vec2(520, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p3[0] != 'N')
	{
		createTextLabel("P3", glm::vec2(720, 500), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p3.substr(1, 2), glm::vec2(720, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p3.substr(0, 1), glm::vec2(720, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p4[0] != 'N')
	{
		createTextLabel("P4", glm::vec2(920, 500), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p4.substr(1, 2), glm::vec2(920, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p4.substr(0, 1), glm::vec2(920, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
	}

	createTextLabel("Level Select", glm::vec2(620, 200), &m_UIButtons, 1.0f, glm::vec3(1, 1, 1));
	createTextLabel("Main Menu", glm::vec2(620, 150), &m_UIButtons, 1.0f, glm::vec3(1, 1, 1));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
	m_screenState = VICTORY;
	buttonReleasedOnce = false;
}

EndScreen::~EndScreen()
{

}

void EndScreen::update()
{
	for (auto& system : m_activeSystems) {
		system->beginFrame();
	}

	for (auto& system : m_activeSystems) {
		system->update();
	}

	// Check inputs
	if (m_bUIActive)
	{
		checkControllerInputForUI();
		for (int i = 0; i < m_UIButtons.size(); ++i)
			m_UIButtons.at(i).Render();
		for (int i = 0; i < m_UITexts.size(); ++i)
			m_UITexts.at(i).Render();
	}

	for (auto& system : m_activeSystems) {
		system->endFrame();
	}

	if (!buttonReleasedOnce && !m_bMenuButtonPressed)
	{
		buttonReleasedOnce = true;
	}
}

void EndScreen::buttonPressed()
{
	// Start Pressed
	if (buttonReleasedOnce)
	{
		if (m_iActiveMenuButtonNumber == 0)
		{
			m_screenToTransitionTo = LEVELSELECT;
			m_bChangeScreen = true;
		}
		// Back Pressed
		else if (m_iActiveMenuButtonNumber == 1)
		{
			m_screenToTransitionTo = MAINMENU;
			m_bChangeScreen = true;
		}

		// Play a sound
		Audio& audio = Audio::getInstance();
		audio.playSFX(BUTTON_CLICKED);
	}
}