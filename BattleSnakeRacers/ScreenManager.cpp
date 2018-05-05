#include "ScreenManager.h"

#include "Screen.h"
#include "GameplayScreen.h"
#include "MainMenuScreen.h"

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
	}
}

void ScreenManager::switchScreen(std::unique_ptr<Screen> screen)
{
	g_currentScreen = std::move(screen);
}
