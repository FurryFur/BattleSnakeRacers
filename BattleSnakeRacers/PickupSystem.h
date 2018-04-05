#pragma once

#include "Entity.h"
#include "System.h"

class Scene;

class PickupSystem : public System {
public:
	PickupSystem(Scene&);

	void update(Entity&) override;

	void beginFrame() override {};
	void endFrame() override {};
};