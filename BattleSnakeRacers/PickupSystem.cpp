#define _USE_MATH_DEFINES

#include "PickupSystem.h"

#include "Entity.h"
#include "Clock.h"
#include "PrimitivePrefabs.h"
#include "Audio.h"

#include <iostream>

glm::vec3 colorFromChar(char _c);

PickupSystem::PickupSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System{ scene }
	, m_playerList{playerList}
{
}

void PickupSystem::update()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& entity = m_scene.getEntity(i);

		if (!entity.hasComponents(COMPONENT_PICKUP, COMPONENT_TRANSFORM))
			continue;
		// The pick up can be picked up
		if (entity.pickup.isActive)
		{
			entity.transform.eulerAngles.y += 2 * Clock::getDeltaTime();

			// Checks if a player and the pickup collide
			for (int i = 0; i < m_playerList.size(); ++i)
			{

				if (m_playerList[i]->transform.position.x >= entity.transform.position.x - 2 && m_playerList[i]->transform.position.x <= entity.transform.position.x + 2)
				{
					if (m_playerList[i]->transform.position.z >= entity.transform.position.z - 2 && m_playerList[i]->transform.position.z <= entity.transform.position.z + 2)
					{
						entity.pickup.isActive = false;
						entity.pickup.respawnTimeStamp = glfwGetTime();

						// Spawn a tail for the player
						TransformComponent snakeTailTransform{};
						Entity& snakeTail = Prefabs::createModel(m_scene, "Assets/Models/hotdog/model.obj", snakeTailTransform);
						

						snakeTail.model.materials.at(3).debugColor = colorFromChar(m_playerList[i]->playerStats.color);

						snakeTail.preTransform.eulerAngles.y = glm::half_pi<float>();
						snakeTail.transform.scale = glm::vec3(4.0f, 4.0f, 4.0f);
						snakeTail.addComponents(COMPONENT_SNAKETAIL, COMPONENT_PHYSICS, COMPONENT_CONTROL);

						// Play a sound
						Audio& audio = Audio::getInstance();
						audio.playSFX(PICKUP_COLLECTED);

						// No current snake tails, follow the player
						if (m_playerList[i]->playerStats.snakeTails.size() == 0)
							snakeTail.snakeTail.entityToFollow = m_playerList[i];

						// Else, follow the last snake tail in the queue
						else
							snakeTail.snakeTail.entityToFollow = m_playerList[i]->playerStats.snakeTails.at(m_playerList[i]->playerStats.snakeTails.size() - 1);

						// Add the snake tail to the player's list of tails
						m_playerList[i]->playerStats.snakeTails.push_back(&snakeTail);

						//Set the players position and velocity to the snake tale
						snakeTail.transform.position = snakeTail.snakeTail.entityToFollow->transform.position;
						//snakeTail.transform.position = entity.transform.position;
						snakeTail.physics.velocity = snakeTail.snakeTail.entityToFollow->physics.velocity;

						// Set the owner of the snake tail to the player
						snakeTail.snakeTail.player = m_playerList[i];
					}
				}
			}
		}
		// The pick up is respawning
		else if (glfwGetTime() >= entity.pickup.respawnTimeStamp + entity.pickup.respawnTimeOffset)
		{
			entity.pickup.isActive = true;
		}
	}
}

glm::vec3 colorFromChar(char _c)
{
	if (_c == 'r')
	{
		return glm::vec3(1, 0, 0);
	}
	else if (_c == 'p')
	{
		return glm::vec3(0.8, 0, 0.8);
	}
	else if (_c == 'b')
	{
		return glm::vec3(0, 0, 1);
	}
	else if (_c == 'y')
	{
		return glm::vec3(1, 1, 0);
	}
	else
	{
		return glm::vec3(0);
	}
}
