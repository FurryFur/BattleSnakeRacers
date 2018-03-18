#include "Game.h"

#include "GLUtils.h"

#include <GLFW\glfw3.h>

GLFWwindow* g_window;

GLFWwindow* Game::getWindowContext()
{
	return g_window;
}

void Game::init()
{
	g_window = GLUtils::initOpenGL();

	Game::preloadModelsAndTextures();
}

// Load game specific models and textures into GPU memory here
void Game::preloadModelsAndTextures()
{

}


void Game::executeOneFrame()
{

}
