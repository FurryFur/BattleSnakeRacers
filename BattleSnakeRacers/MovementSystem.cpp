//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles movement based on an
//                entities input state.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "MovementSystem.h"

#include "MovementComponent.h"
#include "PrimitivePrefabs.h"
#include "GLUtils.h"
#include "GLMUtils.h"
#include "Scene.h"
#include "Entity.h"
#include "Clock.h"
#include "Utils.h"
#include "GLMUtils.h"
#include "Audio.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <cmath>

using namespace glm;

MovementSystem::MovementSystem(Scene& scene)
	: System{ scene }
{
}

void MovementSystem::update()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& entity = m_scene.getEntity(i);

		// Filter movable
		const size_t kMovableMask = COMPONENT_MOVEMENT | COMPONENT_INPUT | COMPONENT_TRANSFORM;
		if (!entity.hasComponents(kMovableMask))
			continue;

		// Only control the player if they are not dead
		if (entity.playerStats.getDeathState() == false)
		{
			// Update facing direction
			// TODO: Only do this if on the ground
			entity.transform.eulerAngles.y -= entity.input.turnAxis * 0.0005f * length(entity.physics.velocity);

			// TODO: Make sideways drag higher so the car can't slide sideways (like it's on ice) when not accelerating

			// Apply turning force
			//vec3 steeringDir = normalize(vec3{ -entity.physics.velocity.y, 0, entity.physics.velocity.x });
			//float velocityMag = length(entity.physics.velocity);
			//entity.physics.acceleration += entity.input.turnAxis * steeringDir * velocityMag;
			// TODO: Add max steering amount to movement component

			// Get orientation vectors
			vec3 forward = glm::rotateY(vec4{ 0, 0, 1, 0 }, entity.transform.eulerAngles.y);
			vec3 right = glm::rotateY(vec4{ -1, 0, 0, 0 }, entity.transform.eulerAngles.y);

			// Project acceleration onto right vector and apply static and dynamic friction in this direction
			// TODO: Add sideways friction to movement component
			const float sidewaysMaxSpeedBeforeTractionLoss = 2.5f;
			const float sidewaysDynamicFrictionMag = 50.0f;
			vec3 sidewaysForce = glm::dot(right, entity.physics.acceleration) * right;
			vec3 sidewaysVelocity = glm::dot(right, entity.physics.velocity) * right;
			vec3 sidewaysVelocityDir = glm::normalize(sidewaysVelocity);
			vec3 frictionForce = vec3(0, 0, 0);
			if (glm::length(sidewaysVelocity) > sidewaysMaxSpeedBeforeTractionLoss)
				entity.physics.acceleration -= sidewaysVelocityDir * sidewaysDynamicFrictionMag;
			else
				entity.physics.velocity -= sidewaysVelocity;

			if (entity.input.acceleratorDown) {
				// Apply acceleration force
				// TODO: Obay a max speed and acceleration variable set in the movement component
				//Audio& audio = Audio::getInstance();
				//audio.playSFX(PLAYER_ENGINE);

				entity.physics.acceleration += forward * 10.0f;
			}
			else
			{
				//Audio& audio = Audio::getInstance();
				//audio.stopAccelerate();
			}
			

			if (entity.input.brakeDown) {
				// TODO: Obay a max reverse speed and acceleration variable set in the movement component
				
				entity.physics.acceleration += -entity.physics.velocity * 5.0f;
			}
		}
	}
}