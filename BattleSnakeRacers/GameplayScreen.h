#pragma once

#include "Screen.h"
#include "TextLabel.h"
#include "ScreenManager.h"

#include <vector>
class Entity;
class GameplayScreen : public Screen
{
public:
	GameplayScreen();
	~GameplayScreen() override;
	void update() override;
private:
	std::vector<Entity*> m_playerList;
	std::vector<TextLabel> m_playerScores;

};

