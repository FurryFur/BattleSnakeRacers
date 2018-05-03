#pragma once

#include "System.h"

#include <vector>

class PlayerSpawnSystem : public System
{
public:
	PlayerSpawnSystem(Scene&, std::vector<Entity*>& _playerList);

	void update(Entity&) override;
	void beginFrame() override {};
	void endFrame() override {};

	void respawn();
	//void addSpawnPoint(Entity);
private:
	std::vector<Entity*>& m_playerList;
	std::vector<Entity> m_spawnPointList;
	glm::vec3 m_spawnPoint;
	
};