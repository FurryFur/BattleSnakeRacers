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
	void buttonPressed() override;
private:
};

