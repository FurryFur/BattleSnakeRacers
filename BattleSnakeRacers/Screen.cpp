#include "Screen.h"

#include "TextLabel.h"

void Screen::update()
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
}

void Screen::createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale, glm::vec3 color)
{
	TextLabel label(labelText, "Assets/Fonts/NYCTALOPIATILT.TTF");
	label.setScale(scale);
	label.setPosition(position);
	label.setColor(color);
	screen->push_back(label);
}

void Screen::setNewActiveButton(bool moveForward)
{
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(0.8f, 0.8f, 0.8f));
	if (moveForward)
	{
		if (m_iActiveMenuButtonNumber == m_UIButtons.size() - 1)
			m_iActiveMenuButtonNumber = 0;
		else
			++m_iActiveMenuButtonNumber;
	}
	else
	{
		if (m_iActiveMenuButtonNumber == 0)
			m_iActiveMenuButtonNumber = m_UIButtons.size() - 1;
		else
			--m_iActiveMenuButtonNumber;
	}
	m_UIButtons.at(m_iActiveMenuButtonNumber).setColor(glm::vec3(1.0f, 1.0f, 0.0f));
}

// Handles what happens when a button is pressed, overridden in each screen
void Screen::buttonPressed()
{
}

void Screen::checkControllerInputForUI()
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

ScreenState Screen::getCurrentScreenState()
{
	return m_screenState;
}

bool Screen::checkForScreenTransition()
{
	return m_bChangeScreen;
}

ScreenState Screen::getTransitionScreen()
{
	return m_screenToTransitionTo;
}

bool Screen::getP2State()
{
	return m_p2Ready;
}

bool Screen::getP3State()
{
	return m_p3Ready;
}

bool Screen::getP4State()
{
	return m_p4Ready;
}

int Screen::getLevelIDNum()
{
	return m_levelIDNum;
}

std::string Screen::getDataForNextScreen()
{
	return m_dataForNextScreen;
}
