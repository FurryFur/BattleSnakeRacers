#pragma once

#include "System.h"
#include <vector>

class Scene;
class Entity;

class SnakeTailSystem : public System {
public:
	SnakeTailSystem(Scene&, std::vector<Entity*>& playerList);

	void update(Entity&) override;
	void beginFrame() override {};
	void endFrame() override {};
private:
	std::vector<Entity*>& m_playerList;
};