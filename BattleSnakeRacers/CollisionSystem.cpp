#include "CollisionSystem.h"

#include "Audio.h"

#include <glm\glm.hpp>

using namespace glm;

PlayerCollisionSystem::PlayerCollisionSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System(scene)
	, m_playerList{ playerList }
{
}

void PlayerCollisionSystem::update()
{
	// Loop through every pair of objects
	for (size_t i = 0; i < m_playerList.size(); ++i) {
		for (size_t j = i + 1; j < m_playerList.size(); ++j) {
			Entity& player1 = *m_playerList.at(i);
			Entity& player2 = *m_playerList.at(j);

			if (!player1.hasComponents(COMPONENT_PHYSICS) || !player2.hasComponents(COMPONENT_PHYSICS)
			 || !player1.hasComponents(COMPONENT_TRANSFORM) || !player2.hasComponents(COMPONENT_TRANSFORM))
				continue;

			// TODO: Implement collision between differing types of collision bodies
			if (player1.hasComponents(COMPONENT_SPHERE_COLLISION) && player2.hasComponents(COMPONENT_SPHERE_COLLISION)) {
				ContactInfo contactInfo = doSphereCollisionDetection(player1, player2);
				if (contactInfo.isCollision) {
					// TODO: Add restitution to collision types instead of hard coding spring constant here
					const float springConstant = 1000;
					glm::vec3 newPlayer1Acceleration = contactInfo.contactNormals[0] * contactInfo.penetrationDistance * springConstant;

					player1.physics.acceleration += newPlayer1Acceleration;
					player2.physics.acceleration += contactInfo.contactNormals[1] * contactInfo.penetrationDistance * springConstant;

					// Play a sound
					glm::vec3 positionOffset = player2.transform.position - player1.transform.position;
					positionOffset = glm::normalize(positionOffset);
					float relativeSpeed = glm::dot(player2.physics.velocity - player1.physics.velocity, positionOffset);
					if (relativeSpeed > 10.0f)
					{
						Audio& audio = Audio::getInstance();
						audio.playSFX(PLAYER_COLLISION);
					}
				}
			}
		}
	}
}

void PlayerCollisionSystem::beginFrame()
{
}

void PlayerCollisionSystem::endFrame()
{
}

ContactInfo PlayerCollisionSystem::doSphereCollisionDetection(Entity& entity1, Entity& entity2)
{
	ContactInfo contactInfo;
	contactInfo.isCollision = false;

	vec3 displacement = entity1.transform.position - entity2.transform.position;
	displacement.y = 0; // Only do 2D collisions
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

//ContactInfo PlayerCollisionSystem::doBoxCollisionDetection(Entity& entity1, Entity& entity2)
//{
//	return ContactInfo();
//}
//
//ContactInfo PlayerCollisionSystem::doCapsuleCollisionDetection(Entity& entity1, Entity& entity2)
//{
//	return ContactInfo();
//}