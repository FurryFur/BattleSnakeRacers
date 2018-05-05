#pragma once

#include "Screen.h"
#include "TextLabel.h"
#include <vector>
class Entity;
class MainMenuScreen : public Screen
{
public:
	MainMenuScreen();
	~MainMenuScreen() override;
	void setNewActiveButton(bool moveForward);
	void buttonPressed();
	void controllerInput();
	void update() override;
private:
	std::vector<TextLabel> m_UIButtons; // The buttons 
	std::vector<TextLabel> m_UITexts; // The static Text in the menu
	int m_iActiveButtonNumber; // The index of which button is active in the vector of buttons
	bool m_buttonPressed;
	bool m_axisPressed;
};

