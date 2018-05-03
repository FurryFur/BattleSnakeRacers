#pragma once

class Entity;

#include <glm\glm.hpp>
#include <vector>

struct PlayerStatsComponent {
	std::vector<Entity*> snakeTails;
	bool isDead;
};