#include "ScreenManager.h"

#include "Screen.h"
#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "ControlsScreen.h"

std::unique_ptr<Screen> g_currentScreen;

void ScreenManager::update()
{
	g_currentScreen->update();

	// Check to see if a new screen should be loaded dependant on the players input in the screen
	if (g_currentScreen->checkForScreenTransition())
	{
		if(g_currentScreen->getTransitionScreen() == GAMEPLAY)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new GameplayScreen));
		else if (g_currentScreen->getTransitionScreen() == MAINMENU)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new MainMenuScreen));
		else if (g_currentScreen->getTransitionScreen() == CONTROLS)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new ControlsScreen));
		else if (g_currentScreen->getTransitionScreen() == QUIT)
		{
			// Quit the game
		}

	}
}

void ScreenManager::switchScreen(std::unique_ptr<Screen> screen)
{
	g_currentScreen = std::move(screen);
}
