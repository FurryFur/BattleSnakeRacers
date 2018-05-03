#pragma once

#include "System.h"

class Scene;

class CollisionSystem : public System {
public:
	CollisionSystem(Scene&);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

private:
	ContactInfo doSphereCollisionDetection(Entity& entity1, Entity& entity2);
	ContactInfo doBoxCollisionDetection(Entity& entity1, Entity& entity2);
	ContactInfo doCapsuleCollisionDetection(Entity& entity1, Entity& entity2);
};