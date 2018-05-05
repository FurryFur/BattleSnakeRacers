#pragma once

#include "System.h"

#include <vector>

class PlayerSpawnSystem : public System
{
public:
	PlayerSpawnSystem(Scene&, std::vector<Entity*>& _playerList);

	void update() override;
	void beginFrame() override {};
	void endFrame() override {};

	void respawn();
	void addSpawnPoint(glm::vec3);
private:
	std::vector<Entity*>& m_playerList;
	std::vector<glm::vec3> m_spawnPointList;
	glm::vec3 m_spawnPoint;
	char spawnDir;
	unsigned int spawnpri;
	
};