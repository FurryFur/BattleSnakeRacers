#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen()
{
	// Create the main menu Start button
	createTextLabel("Start", glm::vec2(637.0f, 312.0f), &m_UIButtons);
	// Create the main menu controls button
	createTextLabel("Controls", glm::vec2(632.0f, 212.0f), &m_UIButtons);
	// Create the main menu quit button
	createTextLabel("Quit", glm::vec2(637.0f, 112.0f), &m_UIButtons);

	// Create the main menu title text
	createTextLabel("Battle Snake", glm::vec2(350.0f, 550.0f), &m_UIButtons, 2.0f);
	// Create the main menu sub title text
	createTextLabel("Racers", glm::vec2(800.0f, 500.0f), &m_UIButtons, 0.8f);
	// Create the main menu credits text
	createTextLabel("Made by: Lance Chaney, Jack Mair, Hugo Adams, Blair Corban", glm::vec2(830.0f, 10.0f), &m_UIButtons, 0.5f);
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::update()
{
	for (size_t i = 0; i < m_UIButtons.size(); ++i)
	{
		m_UIButtons.at(i).Render();
	}
}