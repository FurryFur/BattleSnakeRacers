#define _USE_MATH_DEFINES

#include "PickupSystem.h"

#include "Entity.h"
#include "Clock.h"

#include <iostream>

PickupSystem::PickupSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System{ scene }
	, m_playerList{playerList}
{
}

void PickupSystem::update(Entity& entity)
{
	if (!entity.hasComponents(COMPONENT_PICKUP, COMPONENT_TRANSFORM))
		return;
	if (entity.pickup.isActive)
	{
		entity.transform.eulerAngles.y += 2 * Clock::getDeltaTime();
		if (m_playerList[0]->transform.position.x >= -22 && m_playerList[0]->transform.position.x <= -18)
		{
			if (m_playerList[0]->transform.position.z >= -22 && m_playerList[0]->transform.position.z <= -18)
			{
				std::cout << "pickup\n";
			}
		}
	}
}
