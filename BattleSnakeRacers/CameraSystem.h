#pragma once

#include "System.h"

class Entity;
class Scene;

class CameraSystem : public System
{
public:
	CameraSystem(Scene&);
	~CameraSystem();

	// Inherited via System
	virtual void update(Entity&) override;
	virtual void beginFrame() override;
	virtual void endFrame() override;
};

