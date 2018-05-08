#include "CollisionSystem.h"

#include "Audio.h"

#include <glm\glm.hpp>

using namespace glm;

CollisionSystem::CollisionSystem(Scene& scene)
	: System(scene)
{
}

void CollisionSystem::update()
{
	// Loop through every pair of objects
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		for (size_t j = i + 1; j < m_scene.getEntityCount(); ++j) {
			Entity& entity1 = m_scene.getEntity(i);
			Entity& entity2 = m_scene.getEntity(j);

			if (!entity1.hasComponents(COMPONENT_PHYSICS) || !entity2.hasComponents(COMPONENT_PHYSICS)
			 || !entity1.hasComponents(COMPONENT_TRANSFORM) || !entity2.hasComponents(COMPONENT_TRANSFORM))
				continue;

			// TODO: Implement collision between differing types of collision bodies
			if (entity1.hasComponents(COMPONENT_SPHERE_COLLISION) && entity2.hasComponents(COMPONENT_SPHERE_COLLISION)) {
				ContactInfo contactInfo = doSphereCollisionDetection(entity1, entity2);
				if (contactInfo.isCollision) {
					// TODO: Add restitution to collision types instead of hard coding spring constant here
					const float springConstant = 1000;
					entity1.physics.acceleration += contactInfo.contactNormals[0] * contactInfo.penetrationDistance * springConstant;
					entity2.physics.acceleration += contactInfo.contactNormals[1] * contactInfo.penetrationDistance * springConstant;

					Audio& audio = Audio::getInstance();
					audio.playSFX(PLAYER_CRASH);
				}
			}
		}
	}
}

void CollisionSystem::beginFrame()
{
}

void CollisionSystem::endFrame()
{
}

ContactInfo CollisionSystem::doSphereCollisionDetection(Entity& entity1, Entity& entity2)
{
	ContactInfo contactInfo;
	contactInfo.isCollision = false;

	vec3 displacement = entity1.transform.position - entity2.transform.position;
	float distance = glm::length(displacement);
	float sumRadii = entity1.sphereCollision.radius + entity2.sphereCollision.radius;
	if (distance < sumRadii) {
		contactInfo.isCollision = true;
		contactInfo.contactNormals[0] = glm::normalize(displacement);
		contactInfo.contactNormals[1] = -contactInfo.contactNormals[0];
		contactInfo.penetrationDistance = sumRadii - distance;
	}

	return contactInfo;
}

ContactInfo CollisionSystem::doBoxCollisionDetection(Entity& entity1, Entity& entity2)
{
	return ContactInfo();
}

ContactInfo CollisionSystem::doCapsuleCollisionDetection(Entity& entity1, Entity& entity2)
{
	return ContactInfo();
}