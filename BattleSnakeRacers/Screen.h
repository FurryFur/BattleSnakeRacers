#pragma once

#include "Scene.h"
#include "System.h"

#include <vector>
#include <memory>

enum screenState
{
	NOSTATE = 0,
	MAINMENU,
	CONTROLS,
	PLAYERSELECT,
	LEVELSELECT,
	GAMEPLAY,
	VICTORY
};

class Screen
{
public:
	virtual ~Screen() {};
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

	virtual void update();

	void createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale = 1.0f);
	bool checkForScreenTransition(); // Checks to see if the screen should be transitioned
	screenState getTransitionScreen(); // Gets the screen the screen manager should transition to
protected:
	Screen() {};

	Scene m_scene;
	std::vector<std::unique_ptr<System>> m_activeSystems;
	
	screenState m_screenToTransitionTo;
	bool m_bChangeScreen;
};

