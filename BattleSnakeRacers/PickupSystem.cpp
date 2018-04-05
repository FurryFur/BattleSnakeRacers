#define _USE_MATH_DEFINES

#include "PickupSystem.h"

#include "Clock.h"

PickupSystem::PickupSystem(Scene& scene)
	: System{ scene }
{
}

void PickupSystem::update(Entity& entity)
{
	if (!entity.hasComponents(COMPONENT_PICKUP, COMPONENT_TRANSFORM))
		return;
	if (entity.pickup.isActive)
	{
		

	}
}
