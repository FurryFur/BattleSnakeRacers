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
	void update() override;
private:
	std::vector<TextLabel> m_UIButtons;
};

