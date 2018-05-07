#pragma once

#include "Screen.h"
#include "TextLabel.h"
#include <vector>
class Entity;
class LevelSelectScreen : public Screen
{
public:
	LevelSelectScreen();
	~LevelSelectScreen() override;
	void checkControllerInput();
	void buttonPressed() override;
	void updateLevelText();
	void update() override;
private:
	bool m_horizontalAxisDown;
};

