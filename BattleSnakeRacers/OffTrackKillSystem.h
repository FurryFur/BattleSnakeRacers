#pragma once

#include "System.h"

#include <vector>

class Scene;
class Entity;

class OffTrackKillSystem : public System {
public:
	OffTrackKillSystem(Scene&, const std::vector<Entity*>& playerList, float killY = -50);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

private:
	const std::vector<Entity*>& m_playerList;
	const float m_killY;
};