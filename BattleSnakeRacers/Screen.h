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

	// Controlling UI functions
	void setNewActiveButton(bool moveForward);
	virtual void buttonPressed();
	void checkControllerInputForUI();
	void createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale = 1.0f, glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f));
	bool checkForScreenTransition(); // Checks to see if the screen should be transitioned
	screenState getTransitionScreen(); // Gets the screen the screen manager should transition to
protected:
	Screen() {};

	Scene m_scene;
	std::vector<std::unique_ptr<System>> m_activeSystems;

	std::vector<TextLabel> m_UIButtons; // The buttons 
	std::vector<TextLabel> m_UITexts; // The static Text in the menu
	int m_iActiveMenuButtonNumber; // The index of which button is active in the vector of buttons
	bool m_bMenuButtonPressed; // bool to flag if a menu button has been pressed
	bool m_bMenuAxisPressed; // bool to flag if the axis on the controller has been 'pressed'

	screenState m_screenToTransitionTo;
	bool m_bChangeScreen;
	bool m_bUIActive; // a bool to flag if the UI is active or not;
};

