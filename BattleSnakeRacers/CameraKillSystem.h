#pragma once

#include "System.h"

#include <vector>

class Scene;
class Entity;

// Kills players if they go outside the screen bounds
class CameraKillSystem : public System {
public:
	CameraKillSystem(Scene& scene, const std::vector<Entity*>& playerList, Entity* killCamera = nullptr);

	void setKillCamera(Entity* killCamera);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

private:
	Entity* m_killCamera;
	const std::vector<Entity*>& m_playerList;
};