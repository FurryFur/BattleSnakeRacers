#define _USE_MATH_DEFINES

#include "SnakeTailSystem.h"

#include "Entity.h"
#include "Clock.h"
#include "PrimitivePrefabs.h"
#include "Entity.h"
#include "AIUtils.h"

#include <iostream>

SnakeTailSystem::SnakeTailSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System{ scene }
	, m_playerList{ playerList }
{

}

void SnakeTailSystem::update(Entity& entity)
{
	if (!entity.hasComponents(COMPONENT_SNAKETAIL))
		return;

	// Check to see if the tail has a leader to follow
	if (entity.snakeTail.entityToFollow != NULL)
	{
		// Update the snake tails position to follow its leader
		glm::vec3 acceleration = seekWithArrival(entity.snakeTail.entityToFollow->transform.position, entity.transform.position, entity.physics.velocity, entity.controlVars.maxMoveSpeed);
		steer(entity, acceleration);
	}

	// Check if the snake tail collides with anyother player
	// Cycle over all players
	for (int i = 0; i < m_playerList.size(); ++i)
	{
		// Check that its not the player the snake tail belongs to
		if (m_playerList[i] != entity.snakeTail.player)
		{
			// Check they collide
			if ((m_playerList[i]->transform.position.x >= entity.transform.position.x - 2) 
			 && (m_playerList[i]->transform.position.x <= entity.transform.position.x + 2)
			 && (m_playerList[i]->transform.position.z >= entity.transform.position.z - 2)
			 && (m_playerList[i]->transform.position.z <= entity.transform.position.z + 2))
			{
				// Kill the player
				m_playerList[i]->playerStats.isDead = true;

				// Update the players score
			}
		}
	}
}
