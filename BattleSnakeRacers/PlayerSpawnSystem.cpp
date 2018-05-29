#include "PlayerSpawnSystem.h"

#include "Entity.h"
#include "PrimitivePrefabs.h"
#include "Game.h"
#include "CameraKillSystem.h"
#include "TaskDeferrer.h"

#include <iostream>

PlayerSpawnSystem::PlayerSpawnSystem(Scene& _scene, std::vector<Entity*>& _playerList, CameraKillSystem& cameraKillSystem)
	: System{_scene}
	, m_playerList{ _playerList }
	, m_cameraKillSystem{ cameraKillSystem }
{
	m_spawnPoint = glm::vec3(0);
	for (int i = 0; i < 4;i++)
	{
		if (i < m_playerList.size())
		{
			m_playerDied[i] = 0;
		}
		else
		{
			m_playerDied[i] = -1;
		}
	}
	m_numPlayersDead = 0;
}


void PlayerSpawnSystem::update()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& entity = m_scene.getEntity(i);

		if (!entity.hasComponents(COMPONENT_TRANSFORM, COMPONENT_LEVELPROG))
		{
			continue;
		}


		int mostlaps = -1;
		int mosthighprog = -1;
		int index = 0;
		for (int i = 0; i < m_playerList.size(); ++i)
		{
			if (m_playerList[i]->playerStats.getDeathState() == true)
			{
				if (m_playerDied[i] == 0)
				{
					m_numPlayersDead++;
					m_playerDied[i] = m_numPlayersDead;
				}
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


		if (!m_waitingOnDeferredRespawn && m_numPlayersDead >= m_playerList.size() - 1 )//|| m_spawnPoint != glm::vec3(0))
		{
			m_waitingOnDeferredRespawn = true;
			TaskDeferrer::after(1.5, std::bind(&PlayerSpawnSystem::respawn, this));
		}
	}
}

void PlayerSpawnSystem::respawn()
{
	m_cameraKillSystem.disableFor(5);

	updateScores();

	float rotation;
	bool xSpread = false;
	switch (spawnDir)
	{
	case '>':
		rotation = 180;
		xSpread = false;
		break;
	case 'v':
		rotation = 90;
		xSpread = true;
		break;
	case '<':
		rotation = 0;
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

	rotation -= 90;
	rotation = glm::radians(rotation);

	for (int i = 0; i < m_playerList.size(); ++i)
	{
		Game::resetPlayer(*m_playerList[i]);
		m_playerList[i]->transform.position = m_spawnPoint;
		if (xSpread)
		{
			m_playerList[i]->transform.position.x = m_spawnPoint.x - 10 + i* 5;
		}
		else
		{
			m_playerList[i]->transform.position.z = m_spawnPoint.z - 10 + i* 5;
		}
		m_playerList[i]->transform.position.y = m_spawnPoint.y;
		m_playerList[i]->physics.velocity = glm::vec3{ 0, 0, 0 };
		m_playerList[i]->physics.angularVelocity = glm::vec3{ 0, 0, 0 };
		
		m_playerList[i]->transform.eulerAngles.y = rotation;
		m_playerList[i]->playerStats.setDeathState(false);
		m_playerList[i]->playerStats.lap = 0;
		m_playerList[i]->playerStats.highestProgress = 0;

		for (int j = 0; j < m_playerList[i]->playerStats.snakeTails.size(); ++j)
		{
			m_scene.destroyEntity(*m_playerList[i]->playerStats.snakeTails.at(j));
		}
		m_playerList[i]->playerStats.snakeTails.clear();
	}

	// Reset spawn controller values
	m_numPlayersDead = 0;
	m_waitingOnDeferredRespawn = false;

	for (int i = 0; i < 4; ++i)
	{
		m_playerDied[i] = 0;
	}
}

void PlayerSpawnSystem::addSpawnPoint(glm::vec3 pos)
{
	m_spawnPointList.push_back(pos);
}

void PlayerSpawnSystem::updateScores()
{
	if (m_playerList.size() == 2)
	{
		for (int i = 0; i < 2; i++)
		{
			switch (m_playerDied[i])
			{
			case 0:
				m_playerList[i]->playerStats.currentScore += 1;
				break;
			case 1:
				m_playerList[i]->playerStats.currentScore += -1;
				break;
			default:
				m_playerList[i]->playerStats.currentScore += 1;
				break;
			}
		}
	}
	else if (m_playerList.size() == 3)
	{
		for (int i = 0; i < 3; i++)
		{
			switch (m_playerDied[i])
			{
			case 0:
				m_playerList[i]->playerStats.currentScore += 1;
				break;
			case 1:
				m_playerList[i]->playerStats.currentScore += -1;
				break;
			case 2:
				m_playerList[i]->playerStats.currentScore += 0;
				break;
			default:
				m_playerList[i]->playerStats.currentScore += 1;
				break;
			}
		}
	}
	else if (m_playerList.size() == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			switch (m_playerDied[i])
			{
			case 0:
				m_playerList[i]->playerStats.currentScore += 2;
				break;
			case 1:
				
				m_playerList[i]->playerStats.currentScore += -2;
				break;
			case 2:
				m_playerList[i]->playerStats.currentScore += -1;
				break;
			case 3:
				
				// If at 9 points, do not get points for coming second
				if (m_playerList[i]->playerStats.currentScore != 9)
				{
					m_playerList[i]->playerStats.currentScore += 1;
				}
				break;
			default:
				m_playerList[i]->playerStats.currentScore += 2;
				break;
			}
		}
	}

	// Make sure no player's score is below 0
	for (int i = 0; i < m_playerList.size(); i++)
	{
		if (m_playerList[i]->playerStats.currentScore < 0)
			m_playerList[i]->playerStats.currentScore = 0;
	}
}
	
