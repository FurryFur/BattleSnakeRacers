#include "EndScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "Utils.h"
#include "CameraSystem.h"
#include "GLUtils.h"
#include "TextLabel.h"
#include "Audio.h"

glm::vec3 colorFromChar(std::string _c);
using namespace std;
EndScreen::EndScreen(std::string _dataIn)
{

	string p1 = _dataIn.substr(0, 4);
	string p2 = _dataIn.substr(4, 4);
	string p3 = _dataIn.substr(8, 4);
	string p4 = _dataIn.substr(12, 4);

	int index = 0;
	for (int i = 0; i < _dataIn.length(); i++)
	{
		if (_dataIn[i] == 'W')
		{
			index = i;
			break;
		}
	}
	char pWin = '1' + ((index + 1) / 4);
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

	glm::vec3 color = glm::vec3(1, 1, 1);

	switch (pWin) 
	{
	case '1':
		color = glm::vec3(1, 0, 0);
		break;
	case '2':
		color = colorFromChar(p2.substr(3, 1));
		break;
	case '3':
		color = colorFromChar(p3.substr(3, 1));
		break;
	case '4':
		color = colorFromChar(p4.substr(3, 1));
		break;
	 default:
		break;
	}

	createTextLabel(winMsg, glm::vec2(220, 600), &m_UITexts, 2.0f, color);

	
	if (p1[0] != 'N')
	{
		glm::vec3 col = colorFromChar(p1.substr(3, 1));
		createTextLabel("P1", glm::vec2(320, 500), &m_UITexts, 1.0f, col);
		createTextLabel(p1.substr(1,2), glm::vec2(320, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));		
		createTextLabel(p1.substr(0, 1), glm::vec2(320, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p2[0] != 'N')
	{
		glm::vec3 col = colorFromChar(p2.substr(3, 1));
		createTextLabel("P2", glm::vec2(520, 500), &m_UITexts, 1.0f, col);
		createTextLabel(p2.substr(1, 2), glm::vec2(520, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p2.substr(0, 1), glm::vec2(520, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p3[0] != 'N')
	{
		glm::vec3 col = colorFromChar(p3.substr(3, 1));
		createTextLabel("P3", glm::vec2(720, 500), &m_UITexts, 1.0f, col);
		createTextLabel(p3.substr(1, 2), glm::vec2(720, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p3.substr(0, 1), glm::vec2(720, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));

	}
	if (p4[0] != 'N')
	{
		glm::vec3 col = colorFromChar(p4.substr(3, 1));
		createTextLabel("P4", glm::vec2(920, 500), &m_UITexts, 1.0f, col);
		createTextLabel(p4.substr(1, 2), glm::vec2(920, 400), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
		createTextLabel(p4.substr(0, 1), glm::vec2(920, 300), &m_UITexts, 1.0f, glm::vec3(1, 1, 1));
	}

	createTextLabel("Level Select", glm::vec2(540, 200), &m_UIButtons, 1.0f, glm::vec3(1, 1, 1));
	createTextLabel("Main Menu", glm::vec2(560, 130), &m_UIButtons, 1.0f, glm::vec3(1, 1, 1));

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

glm::vec3 colorFromChar(std::string _c)
{
	if (_c == "r")
	{
		return glm::vec3(1, 0, 0);
	}
	else if (_c == "p")
	{
		return glm::vec3(0.8, 0, 0.8);
	}
	else if (_c == "b")
	{
		return glm::vec3(0, 0, 1);
	}
	else if (_c == "y")
	{
		return glm::vec3(1, 1, 0);
	}
	else
	{
		return glm::vec3(0);
	}
}