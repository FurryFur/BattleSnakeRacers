#pragma once

class Entity;

#include <glm\glm.hpp>
#include <vector>

#include "TextLabel.h"

struct PlayerStatsComponent {
	
	int currentScore = 5;
	TextLabel* scoreLabel;

	//bool isDead;
	unsigned int progression;//checking who is first on the map and their last respwan point
	unsigned int highestProgress;//mainly just for finsihing a lap

	unsigned int lap = 0;
	char color = ' ';

	std::vector<Entity*> snakeTails;
	bool getDeathState()
	{
		return isDead;
	}
	void setDeathState(bool _b)
	{
		isDead = _b;
	}
private:
	bool isDead;
};