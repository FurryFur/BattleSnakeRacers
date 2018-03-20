#define _USE_MATH_DEFINES

#include "PhysicsSystem.h"

#include "RenderSystem.h"

#include <cmath>

PhysicsSystem::PhysicsSystem(Scene& scene)
	: System{ scene }
{
}

void PhysicsSystem::update(Entity& entity)
{
	if (!entity.hasComponents(COMPONENT_PHYSICS, COMPONENT_TRANSFORM))
		return;

	// TODO: Re-implement below with new global clock

	//entity.physics.velocity += entity.physics.acceleration * deltaTick;
	//entity.transform.position += entity.physics.velocity * deltaTick;

	//RenderSystem::drawDebugArrow(entity.lookAt[3], entity.physics.velocity, glm::length(entity.physics.velocity), { 0, 1, 0 });
	//RenderSystem::drawDebugArrow(glm::vec3(entity.lookAt[3]) + entity.physics.velocity, entity.physics.acceleration, glm::length(entity.physics.acceleration));
}
