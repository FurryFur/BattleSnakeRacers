#include "Game.h"

#include "GLUtils.h"
#include "ScreenManager.h"
#include "Clock.h"
#include "MainMenuScreen.h"
#include "ModelUtils.h"
#include "Audio.h"
#include "GameplayScreen.h"

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
	audio.playSFX(PICKUP_COLLECTED);
	ScreenManager::switchScreen(std::unique_ptr<Screen>(new MainMenuScreen()));
	//bool g_playersInGame[4] = { true,true,false,false };
	//std::array<bool, 4> barray= { true,true,true,true };
	//ScreenManager::switchScreen(std::unique_ptr<Screen>(new GameplayScreen(barray,3)));
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

void Game::killPlayer(Entity& player)
{
	player.playerStats.progression = 0;
	player.playerStats.highestProgress = 0;
	player.playerStats.lap = 0;
	player.playerStats.setDeathState(true);
	player.removeComponents(COMPONENT_INPUT, COMPONENT_TRANSFORM, COMPONENT_MODEL);

	player.physics = {};
	player.movement = {};
	player.input = {};
}

void Game::resetPlayer(Entity& player)
{
	player.playerStats.setDeathState(false);
	player.addComponents(COMPONENT_INPUT, COMPONENT_TRANSFORM, COMPONENT_MODEL);
}
