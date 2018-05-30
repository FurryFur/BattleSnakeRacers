#pragma once

#include "System.h"

#include <vector>

class Scene;

class PlayerCollisionSystem : public System {
public:
	PlayerCollisionSystem(Scene&, std::vector<Entity*>& playerList);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

private:
	ContactInfo doSphereCollisionDetection(Entity& entity1, Entity& entity2);
	//ContactInfo doBoxCollisionDetection(Entity& entity1, Entity& entity2);
	//ContactInfo doCapsuleCollisionDetection(Entity& entity1, Entity& entity2);

	std::vector<Entity*>& m_playerList;
};