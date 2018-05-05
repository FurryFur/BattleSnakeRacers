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
	createTextLabel("Back", glm::vec2(40.0f, 700.0f), &m_UIButtons, 0.5f);

	// Create the main menu title text
	createTextLabel("Player Select, connect controllers to play!", glm::vec2(40.0f, 760.0f), &m_UITexts, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	m_centreText = createUniqueTextLabel("Atleast 2 players needed to Play!", glm::vec2(400.0f, 400.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	m_p1Ready = createUniqueTextLabel("P1 Joined!", glm::vec2(200.0f, 600.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	m_p2PressStart = createUniqueTextLabel("P2 press start to join.", glm::vec2(840.0f, 600.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	m_p3PressStart = createUniqueTextLabel("P3 press start to join.", glm::vec2(200.0f, 150.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	m_p4PressStart = createUniqueTextLabel("P4 press start to join.", glm::vec2(840.0f, 150.0f), 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
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
	for (int gamepadIdx = 0; gamepadIdx < 4; ++gamepadIdx)
	{
		int count;

		// Update input from buttons
		const unsigned char* pBtns = glfwGetJoystickButtons(gamepadIdx, &count);
		if (count > 0) {
			std::vector<unsigned char> btns(pBtns, pBtns + count);
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
		}
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
		checkControllerInputForUI();
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