#include "PlayerSelectScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"
#include "KeyObserver.h"

#include <GLFW\glfw3.h>

PlayerSelectScreen::PlayerSelectScreen()
	: m_centreText{ "" , "Assets/Fonts/NYCTALOPIATILT.TTF"}
	, m_p1Ready{ "" , "Assets/Fonts/NYCTALOPIATILT.TTF" }
	,m_p2PressStart{ "" , "Assets/Fonts/NYCTALOPIATILT.TTF" }
	,m_p3PressStart{ "" , "Assets/Fonts/NYCTALOPIATILT.TTF" }
	,m_p4PressStart{ "" , "Assets/Fonts/NYCTALOPIATILT.TTF" }
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

	// Create the main menu quit button
	createTextLabel("Back", glm::vec2(40.0f, 700.0f), &m_UIButtons, 0.5f);

	// Create the main menu title text
	createTextLabel("Player Select, connect controllers to play!", glm::vec2(40.0f, 760.0f), &m_UITexts, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	m_centreText = createUniqueTextLabel("Atleast 2 players needed to Play!", glm::vec2(400.0f, 400.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	m_p1Ready = createUniqueTextLabel("P1 Joined!", glm::vec2(200.0f, 600.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	m_p2PressStart = createUniqueTextLabel("P2 press start to join.", glm::vec2(840.0f, 600.0f), 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	m_p3PressStart = createUniqueTextLabel("P3 press start to join.", glm::vec2(200.0f, 150.0f), 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	m_p4PressStart = createUniqueTextLabel("P4 press start to join.", glm::vec2(840.0f, 150.0f), 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
	m_screenState = PLAYERSELECT;
}

PlayerSelectScreen::~PlayerSelectScreen()
{
}

TextLabel PlayerSelectScreen::createUniqueTextLabel(std::string labelText, glm::vec2 position, float scale, glm::vec3 color)
{
	TextLabel newLabel(labelText, "Assets/Fonts/NYCTALOPIATILT.TTF");
	newLabel.setScale(scale);
	newLabel.setPosition(position);
	newLabel.setColor(color);
	return newLabel;
}

void PlayerSelectScreen::buttonPressed()
{
	// Back Pressed
	if (m_iActiveMenuButtonNumber == 0)
	{
		m_screenToTransitionTo = MAINMENU;
		m_bChangeScreen = true;
	}
}

void PlayerSelectScreen::checkControllerInput()
{
	// Only player 1 can control the menu screen.
	int count;

	// Update input from buttons
	const unsigned char* p1Btns = glfwGetJoystickButtons(0, &count);
	if (count > 0) {
		std::vector<unsigned char> btns(p1Btns, p1Btns + count);
		// Check if the player pressed the A button
		if (!m_bMenuButtonPressed && btns[0] == 1)
		{
			m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			m_bMenuButtonPressed = true;
		}
		// The button has been pressed, trigger the response
		else if (m_bMenuButtonPressed && btns[0] == 0)
		{
			m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
			m_bMenuButtonPressed = false;
			buttonPressed();
		}
		// Player one pressed start and the next screen is triggered to load
		else if (m_bGameCanStart && btns[7] == 1)
		{
			m_screenToTransitionTo = LEVELSELECT;
			m_bChangeScreen = true;
		}
	}
}

// Check p2,p3,p4 input to join/leave the game
void PlayerSelectScreen::checkPlayerReadyInput()
{
	// Player 2
	int count;

	// Update input from buttons
	const unsigned char* p2Btns = glfwGetJoystickButtons(1, &count);

	if (count > 0) {
		std::vector<unsigned char> btns(p2Btns, p2Btns + count);
		// Check if the player pressed the start button
		if (btns[7] == 1)
		{
			m_p2Ready = true;
			m_p2PressStart.setText("P2 Joined!");
			m_p2PressStart.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}
		// Check if the player pressed the b button
		else if (btns[1] == 1)
		{
			m_p2Ready = false;
			m_p2PressStart.setText("P2 press start to join.");
			m_p2PressStart.setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		}
	}

	// Player 3
	// Update input from buttons
	const unsigned char* p3Btns = glfwGetJoystickButtons(2, &count);

	if (count > 0) {
		std::vector<unsigned char> btns(p3Btns, p3Btns + count);
		// Check if the player pressed the start button
		if (btns[7] == 1)
		{
			m_p3Ready = true;
			m_p3PressStart.setText("P2 Joined!");
			m_p3PressStart.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}
		// Check if the player pressed the b button
		else if (btns[1] == 1)
		{
			m_p3Ready = false;
			m_p3PressStart.setText("P2 press start to join.");
			m_p3PressStart.setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		}
	}

	// Player 4
	// Update input from buttons
	const unsigned char* p4Btns = glfwGetJoystickButtons(3, &count);

	if (count > 0) {
		std::vector<unsigned char> btns(p4Btns, p4Btns + count);
		// Check if the player pressed the start button
		if (btns[7] == 1)
		{
			m_p4Ready = true;
			m_p4PressStart.setText("P2 Joined!");
			m_p4PressStart.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		}
		// Check if the player pressed the b button
		else if (btns[1] == 1)
		{
			m_p4Ready = false;
			m_p4PressStart.setText("P2 press start to join.");
			m_p4PressStart.setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		}
	}
}

// Check to see if enough players have joined the game for it to start
bool PlayerSelectScreen::checkEnoughPlayersJoined()
{
	if (m_p2Ready || m_p3Ready || m_p4Ready)
	{
		m_centreText.setText("P1 press start to race!");
		m_centreText.setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		return true;
	}
	else
	{
		m_centreText.setText("Atleast 2 players needed to Play!");
		m_centreText.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		return false;
	}
}

void PlayerSelectScreen::update()
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
		checkControllerInput();
		checkPlayerReadyInput();
		m_bGameCanStart = checkEnoughPlayersJoined();

		for (int i = 0; i < m_UIButtons.size(); ++i)
			m_UIButtons.at(i).Render();
		for (int i = 0; i < m_UITexts.size(); ++i)
			m_UITexts.at(i).Render();

		m_centreText.Render();
		m_p1Ready.Render();
		m_p2PressStart.Render();
		m_p3PressStart.Render();
		m_p4PressStart.Render();
	}
	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}