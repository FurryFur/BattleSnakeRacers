#include "Game.h"

#include "GLUtils.h"
#include "ScreenManager.h"
#include "Clock.h"
#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "ModelUtils.h"
#include "Audio.h"

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

	//ScreenManager::switchScreen(std::unique_ptr<Screen>(new GameplayScreen));
	ScreenManager::switchScreen(std::unique_ptr<Screen>(new MainMenuScreen));
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
