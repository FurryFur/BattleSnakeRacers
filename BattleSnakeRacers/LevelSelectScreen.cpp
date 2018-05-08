#include "LevelSelectScreen.h"

#include "RenderSystem.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "LevelLoader.h"
#include "CameraSystem.h"
#include "Utils.h"
#include "KeyObserver.h"
#include "Audio.h"

#include <GLFW\glfw3.h>

LevelSelectScreen::LevelSelectScreen()
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

	// Create the start button
	createTextLabel("Start", glm::vec2(620.0f, 150.0f), &m_UIButtons);
	// Create the back button
	createTextLabel("Back", glm::vec2(640.0f, 50.0f), &m_UIButtons);
	createTextLabel("Select A LeveL: ", glm::vec2(40.0f, 760.0f), &m_UITexts, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Level 1", glm::vec2(280.0f, 680.0f), &m_UITexts, 1.5f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 1 Description", glm::vec2(230.0f, 600.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 2 Description", glm::vec2(230.0f, 570.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 3 Description", glm::vec2(230.0f, 540.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 4 Description", glm::vec2(230.0f, 510.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 5 Description", glm::vec2(230.0f, 480.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 6 Description", glm::vec2(230.0f, 450.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 7 Description", glm::vec2(230.0f, 420.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 8 Description", glm::vec2(230.0f, 390.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 9 Description", glm::vec2(230.0f, 360.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 10 Description", glm::vec2(230.0f, 330.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 11 Description", glm::vec2(230.0f, 300.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("Line 12 Description", glm::vec2(230.0f, 270.0f), &m_UITexts, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f));
	createTextLabel("<<", glm::vec2(30.0f, 400.0f), &m_UITexts, 1.5f, glm::vec3(1.0f, 1.0f, 0.0f));
	createTextLabel(">>", glm::vec2(1270.0f, 400.0f), &m_UITexts, 1.5f, glm::vec3(1.0f, 1.0f, 0.0f));

	m_activeSystems.push_back(std::move(renderSystem));
	m_iActiveMenuButtonNumber = 0;
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
	m_bUIActive = true;
	m_screenState = LEVELSELECT;
	m_levelIDNum = 0;
}

LevelSelectScreen::~LevelSelectScreen()
{
}

void LevelSelectScreen::checkControllerInput()
{
	// Only player 1 can control the menu screen.
	int gamepadIdx = 0;

	// Update input from axes
	int count;
	const float* pAxes = glfwGetJoystickAxes(gamepadIdx, &count);
	if (count > 0) {
		std::vector<float> axes(pAxes, pAxes + count);
		if (!m_bMenuAxisPressed)
		{
			// Move forward in the list of buttons
			if (axes.at(1) < -0.8f)
			{
				setNewActiveButton(true);
				m_bMenuAxisPressed = true;
				m_bMenuAxisPressed = true;
				m_bMenuButtonPressed = false;
			}
			// Move backwards in the list of buttons
			if (axes.at(1) > 0.8f)
			{
				setNewActiveButton(false);
				m_bMenuAxisPressed = true;
				m_bMenuButtonPressed = false;
			}
		}
		else if ((axes.at(1) < 0.6f) && (axes.at(1) > -0.6f))
			m_bMenuAxisPressed = false;
		if (!m_horizontalAxisDown)
		{
			// Cycle backwards through the levels
			if (axes.at(0) < -0.8f)
			{
				--m_levelIDNum;
				if (m_levelIDNum == -1)
					m_levelIDNum = 2;
				m_horizontalAxisDown = true;
			}

			// Cycle forwards through the levels
			if (axes.at(0) > 0.8f)
			{
				++m_levelIDNum;
				if (m_levelIDNum == 3)
					m_levelIDNum = 0;
				m_horizontalAxisDown = true;
			}
		}
		else if ((axes.at(0) < 0.6f) && (axes.at(0) > -0.6f))
			m_horizontalAxisDown = false;
	}

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

void LevelSelectScreen::buttonPressed()
{
	// Start Pressed
	if (m_iActiveMenuButtonNumber == 0)
	{
		m_screenToTransitionTo = GAMEPLAY;
		m_bChangeScreen = true;
	}
	// Back Pressed
	else if (m_iActiveMenuButtonNumber == 1)
	{
		m_screenToTransitionTo = PLAYERSELECT;
		m_bChangeScreen = true;
	}
	// Play a sound
	Audio& audio = Audio::getInstance();
	audio.playSFX(BUTTON_CLICKED);

}

void LevelSelectScreen::updateLevelText()
{
	if (m_levelIDNum == 0)
	{
		m_UITexts.at(1).setText("Boa Bay");
		m_UITexts.at(2).setText("Before The Collapse, Boa Bay was once a");
		m_UITexts.at(3).setText("shining tourist trap of the Great City.");
		m_UITexts.at(4).setText("Now it is nothing more than a glob of");
		m_UITexts.at(5).setText("polluted water and dirty beach towels,");
		m_UITexts.at(6).setText("forever left out to save sewage stained");
		m_UITexts.at(7).setText("loungers.");
		m_UITexts.at(8).setText("");
		m_UITexts.at(9).setText("Watch out for the mutant population of");
		m_UITexts.at(10).setText("the city, rumours claim they have taken");
		m_UITexts.at(11).setText("over the bay and set up a permanent");
		m_UITexts.at(12).setText("beach rave. They say the water here is");
		m_UITexts.at(13).setText("great for growing their tails.");
	}
	else if (m_levelIDNum == 1)
	{
		m_UITexts.at(1).setText("Cobra Canyon");
		m_UITexts.at(2).setText("The Great City will always need power,");
		m_UITexts.at(3).setText("even after an apocalyptic event.");
		m_UITexts.at(4).setText("Cobra Canyon is where that power is");
		m_UITexts.at(5).setText("generated. The lava pumped up from");
		m_UITexts.at(6).setText("the belly of the earth heating its");
		m_UITexts.at(7).setText("great steam machines above.");
		m_UITexts.at(8).setText("");
		m_UITexts.at(9).setText("The locals welcome the Race Cars here,");
		m_UITexts.at(10).setText("they know the numerous crashes offer");
		m_UITexts.at(11).setText("an abundance of scrap metal they use");
		m_UITexts.at(12).setText("to repair their ancient machines.");
		m_UITexts.at(13).setText("");
	}
	else
	{
		m_UITexts.at(1).setText("Viper Valley");
		m_UITexts.at(2).setText("As the earth heated up after The Collapse,");
		m_UITexts.at(3).setText("the polar caps melted at an even more");
		m_UITexts.at(4).setText("exaggerated rate than before. Soon, only");
		m_UITexts.at(5).setText("one ice cube remained.");
		m_UITexts.at(6).setText("");
		m_UITexts.at(7).setText("Lucky this ice cube was a perfect shape");
		m_UITexts.at(8).setText("for a race track.");
		m_UITexts.at(9).setText("");
		m_UITexts.at(10).setText("Now civilians flock from the Great City");
		m_UITexts.at(11).setText("to watch the most daring racers battle");
		m_UITexts.at(12).setText("their way to victory in this icy arena.");
		m_UITexts.at(13).setText("");
	}
}

void LevelSelectScreen::update()
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
		updateLevelText();

		for (int i = 0; i < m_UIButtons.size(); ++i)
			m_UIButtons.at(i).Render();
		for (int i = 0; i < m_UITexts.size(); ++i)
			m_UITexts.at(i).Render();
	}
	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}