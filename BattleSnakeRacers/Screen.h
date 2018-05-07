#pragma once

#include "Scene.h"
#include "System.h"

#include <vector>
#include <memory>

enum ScreenState
{
	NOSTATE = 0,
	MAINMENU,
	CONTROLS,
	PLAYERSELECT,
	LEVELSELECT,
	GAMEPLAY,
	VICTORY,
	QUIT
};

class Screen
{
public:
	virtual ~Screen() {};
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;
	virtual void update();

	// UI functions
	void setNewActiveButton(bool moveForward);
	virtual void buttonPressed();
	void checkControllerInputForUI();
	ScreenState getCurrentScreenState();
	void createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale = 1.0f, glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f));
	bool checkForScreenTransition(); // Checks to see if the screen should be transitioned
	ScreenState getTransitionScreen(); // Gets the screen the screen manager should transition to

	// Returns if each player is party of the game or not
	bool getP2State();
	bool getP3State();
	bool getP4State();
	int getLevelIDNum();
protected:
	Screen() {};

	Scene m_scene;
	std::vector<std::unique_ptr<System>> m_activeSystems;

	std::vector<TextLabel> m_UIButtons; // The buttons 
	std::vector<TextLabel> m_UITexts; // The static Text in the menu
	size_t m_iActiveMenuButtonNumber; // The index of which button is active in the vector of buttons
	bool m_bMenuButtonPressed; // bool to flag if a menu button has been pressed
	bool m_bMenuAxisPressed; // bool to flag if the axis on the controller has been 'pressed'

	ScreenState m_screenState; // The current screen state of the screen;
	ScreenState m_screenToTransitionTo;
	bool m_bChangeScreen;
	bool m_bUIActive; // a bool to flag if the UI is active or not;

	bool m_p2Ready;
	bool m_p3Ready;
	bool m_p4Ready;

	int m_levelIDNum;
};

