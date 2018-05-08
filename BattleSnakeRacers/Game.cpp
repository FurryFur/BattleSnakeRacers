#include "Game.h"

#include "GLUtils.h"
#include "ScreenManager.h"
#include "Clock.h"
#include "MainMenuScreen.h"
#include "ModelUtils.h"
#include "Audio.h"
//#include "GameplayScreen.h"

#include <GLFW\glfw3.h>

#include <memory>

GLFWwindow* g_window;

GLFWwindow* Game::getWindowContext()
{
	return g_window;
}

void Game::init()
{
	// Init combined Window and OpenGL context.
	g_window = GLUtils::initOpenGL();

	// Preload models and textures
	Game::preloadModelsAndTextures();

	Audio& audio = Audio::getInstance();
	audio.playMenuMusic();
	ScreenManager::switchScreen(std::unique_ptr<Screen>(new MainMenuScreen));
	//bool g_playersInGame[4] = { true,true,false,false };
	//ScreenManager::switchScreen(std::unique_ptr<Screen>(new GameplayScreen(g_playersInGame)));
}

// Load game specific models and textures into GPU memory here
void Game::preloadModelsAndTextures()
{
	ModelUtils::loadModel("Assets/Models/hotdog/model.obj");
}


void Game::executeOneFrame()
{
	Clock::update();
	ScreenManager::update();
}
