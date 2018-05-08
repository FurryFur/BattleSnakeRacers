#include "ScreenManager.h"

#include "Screen.h"
#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "ControlsScreen.h"
#include "PlayerSelectScreen.h"
#include "LevelSelectScreen.h"
#include "EndScreen.h"
#include "Audio.h"

std::unique_ptr<Screen> g_currentScreen;
bool g_playersInGame[4]; // The active players chonsen during player select
int g_levelIDNumber; // The level ID number chosen
Audio& audio = Audio::getInstance();

void ScreenManager::update()
{
	g_currentScreen->update();

	// Check to see if a new screen should be loaded dependant on the players input in the screen
	if (g_currentScreen->checkForScreenTransition())
	{
		if (g_currentScreen->getTransitionScreen() == GAMEPLAY) {
			if (g_currentScreen->getCurrentScreenState() == LEVELSELECT)
			
			g_levelIDNumber = g_currentScreen->getLevelIDNum();

			// Change the music to be gameplay music when in game
			if (g_levelIDNumber == 0)
				audio.playTrack1Music();
			else if (g_levelIDNumber == 1)
				audio.playTrack2Music();
			else if (g_levelIDNumber == 2)
				audio.playTrack3Music();

			ScreenManager::switchScreen(std::unique_ptr<Screen>(new GameplayScreen(g_playersInGame)));
		}
		else if (g_currentScreen->getTransitionScreen() == MAINMENU)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new MainMenuScreen));
		else if (g_currentScreen->getTransitionScreen() == PLAYERSELECT)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new PlayerSelectScreen));
		else if (g_currentScreen->getTransitionScreen() == LEVELSELECT)
		{
			// Update the active players when leaving the player select screen
			if (g_currentScreen->getCurrentScreenState() == PLAYERSELECT)
			{
				g_playersInGame[0] = true;
				g_playersInGame[1] = g_currentScreen->getP2State();
				g_playersInGame[2] = g_currentScreen->getP3State();
				g_playersInGame[3] = g_currentScreen->getP4State();
			}
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new LevelSelectScreen));
		}
		else if (g_currentScreen->getTransitionScreen() == CONTROLS)
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new ControlsScreen));
		else if (g_currentScreen->getTransitionScreen() == VICTORY)
		{
			std::string last = g_currentScreen->getDataForNextScreen();
			ScreenManager::switchScreen(std::unique_ptr<Screen>(new EndScreen(last)));
			audio.playMenuMusic();
		}
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
