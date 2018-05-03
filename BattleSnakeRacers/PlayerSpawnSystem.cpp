#include "PlayerSpawnSystem.h"

#include "Entity.h"
#include "PrimitivePrefabs.h"

#include <iostream>

PlayerSpawnSystem::PlayerSpawnSystem(Scene& _scene, std::vector<Entity*>& _playerList)
	:System{_scene}
, m_playerList{ _playerList }
{
	
}

void PlayerSpawnSystem::update(Entity& _entity)
{
	if (!_entity.hasComponents(COMPONENT_PLAYERSTATS, COMPONENT_TRANSFORM))
	{
		return;
	}

	int counter = 0;
	for (int i = 0; i < m_playerList.size(); ++i)
	{
		if (m_playerList[i]->playerStats.isDead == true)
		{
			counter++;
		}
	}

	if (counter >= m_playerList.size() - 1)
	{
		
	}

}