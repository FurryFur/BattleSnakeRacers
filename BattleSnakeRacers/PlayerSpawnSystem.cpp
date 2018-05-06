#include "PlayerSpawnSystem.h"

#include "Entity.h"
#include "PrimitivePrefabs.h"

#include <iostream>

PlayerSpawnSystem::PlayerSpawnSystem(Scene& _scene, std::vector<Entity*>& _playerList)
	:System{_scene}
, m_playerList{ _playerList }
{
	m_spawnPoint = glm::vec3(0);
}


void PlayerSpawnSystem::update()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& entity = m_scene.getEntity(i);

		if (!entity.hasComponents(COMPONENT_TRANSFORM, COMPONENT_LEVELPROG))
		{
			continue;
		}

		int counter = 0;

		int mostlaps = -1;
		int mosthighprog = -1;
		int index = 0;
		for (int i = 0; i < m_playerList.size(); ++i)
		{
			if (m_playerList[i]->playerStats.isDead == true)
			{
				counter++;
			}
			else
			{
				//check if player is 1st
				//find closest transform of respawn transforms

				float fdistance = 12.5f;;
				//update lap postion
				float a = glm::distance(m_playerList[i]->transform.position, entity.transform.position);
				if (a < fdistance)
				{
					fdistance = a;
					unsigned int progress = m_playerList[i]->playerStats.progression;
					unsigned int highprog = m_playerList[i]->playerStats.highestProgress;
					unsigned int entprog = entity.levelProg.priority;

					if (entprog > highprog)
					{//if further in the track set highest
						m_playerList[i]->playerStats.highestProgress = entprog;
					}
					else if (highprog >= static_cast<unsigned int>('[') && entprog == static_cast<unsigned int>('A'))
					{//if about to finish going round a track
						m_playerList[i]->playerStats.highestProgress = entprog;
						m_playerList[i]->playerStats.lap++;
					}
					m_playerList[i]->playerStats.progression = entity.levelProg.priority;
				}
				
				if (m_playerList[i]->playerStats.lap >= mostlaps)
				{
					if (m_playerList[i]->playerStats.highestProgress >= mosthighprog)
					{
						index = i;
						mostlaps = m_playerList[i]->playerStats.lap;
						mosthighprog = m_playerList[i]->playerStats.highestProgress;
						
					}
				}

			}
		}

		if (m_playerList[index]->playerStats.highestProgress == entity.levelProg.priority)
		{
			if (entity.levelProg.isRespawn == true)
			{
				m_spawnPoint = entity.transform.position;
				spawnDir = entity.levelProg.respawnDirection;
				//std::cout << "new spawn point at " << entity.levelProg.priority<<std::endl;
			}
		}

		if (counter >= m_playerList.size() - 1 )//|| m_spawnPoint != glm::vec3(0))
		{
			respawn();
		}
	}
}

void PlayerSpawnSystem::respawn()
{
	float rotation;
	bool xSpread = false;
	switch (spawnDir)
	{
	case '>':
		rotation = 0;
		xSpread = false;
		break;
	case 'v':
		rotation = 90;
		xSpread = true;
		break;
	case '<':
		rotation = 180;
		xSpread = false;
		break;
	case '^':
		rotation = 270;
		xSpread = true;
		break;
	default:
		rotation = 0;
		break;
	}

	rotation = glm::radians(rotation);

	for (int i = 0; i < m_playerList.size(); ++i)
	{
		m_playerList[i]->transform.position = m_spawnPoint;
		if (xSpread)
		{
			m_playerList[i]->transform.position.x = m_spawnPoint.x - 10 + i* 5;
		}
		else
		{
			m_playerList[i]->transform.position.z = m_spawnPoint.z - 10 + i* 5;
		}

		m_playerList[i]->playerStats.isDead = false;
		m_playerList[i]->playerStats.lap = 0;
		m_playerList[i]->playerStats.highestProgress = 0;
	}
}

void PlayerSpawnSystem::addSpawnPoint(glm::vec3 pos)
{
	m_spawnPointList.push_back(pos);
}