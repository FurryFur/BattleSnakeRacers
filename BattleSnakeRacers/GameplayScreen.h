#pragma once

#include "Screen.h"
#include "TextLabel.h"
#include "ScreenManager.h"

#include <vector>
#include <array>

class Entity;
class GameplayScreen : public Screen
{
public:
	GameplayScreen(std::array<bool, 4> activePlayers, int level);
	~GameplayScreen() override;
	void update() override;
private:
	std::vector<Entity*> m_playerList;
	std::vector<TextLabel> m_playerScores;

};

