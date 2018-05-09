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

	// Disable the camera kill system for a duration.
	// Useful for respawning players so that the camera kill system
	// doesn't kill them while lerping to the respawn location.
	void disableFor(float seconds);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

private:
	Entity* m_killCamera;
	const std::vector<Entity*>& m_playerList;

	// Remaining duration to be disabled for in seconds
	float disabledRemainingDuration;
};