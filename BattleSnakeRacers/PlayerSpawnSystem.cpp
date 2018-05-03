#include "PlayerSpawnSystem.h"

#include "Entity.h"
#include "PrimitivePrefabs.h"

#include <iostream>

PlayerSpawnSystem::PlayerSpawnSystem(Scene& _scene, std::vector<Entity*>& _playerList)
	:System{_scene}
, m_playerList{ _playerList }
{
	
}

void PlayerSpawnSystem::beginFrame()
{
	int counter = 0;
	
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

			float fdistance = 99999.0f;
			int index = 0;
			for (unsigned int j = 0; j < m_spawnPointList.size(); j++)
			{
				float a = glm::distance(m_playerList[i]->transform.position, m_spawnPointList[j]);
				if (a <= fdistance)
				{
					fdistance = a;
					index = j;
				}
			}
			if (fdistance != 99999.0f)
			{
				m_spawnPoint = m_spawnPointList[index];
			}
		}
	}

	if (counter >= m_playerList.size() - 1)
	{
		respawn();
	}

}

void PlayerSpawnSystem::respawn()
{
	for (int i = 0; i < m_playerList.size(); ++i)
	{
		m_playerList[i]->transform.position = m_spawnPoint;
		m_playerList[i]->transform.position.z = m_spawnPoint.z - 2 + i;
		m_playerList[i]->playerStats.isDead = false;
	}
}

void PlayerSpawnSystem::addSpawnPoint(glm::vec3 pos)
{
	m_spawnPointList.push_back(pos);
}