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
	// Init combined Window and OpenGL context.
	g_window = GLUtils::initOpenGL();

	// Preload models and textures
	Game::preloadModelsAndTextures();
}

// Load game specific models and textures into GPU memory here
void Game::preloadModelsAndTextures()
{

}


void Game::executeOneFrame()
{

}
