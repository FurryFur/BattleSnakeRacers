#pragma once

class Entity;

#include <glm\glm.hpp>
#include <vector>

#include "TextLabel.h"

struct PlayerStatsComponent {
	
	int currentScore = 5;
	bool isDead;

	TextLabel* scoreLabel;

	std::vector<Entity*> snakeTails;
};