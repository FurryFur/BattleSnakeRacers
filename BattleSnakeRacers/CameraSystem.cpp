#include "CameraSystem.h"

#include "Entity.h"

#include <glm\glm.hpp>

using namespace glm;

CameraSystem::CameraSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System(scene)
	, m_playerList{ playerList }
{
}


CameraSystem::~CameraSystem()
{
}

void CameraSystem::update()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& cameraEntity = m_scene.getEntity(i);

		if (!cameraEntity.hasComponents(COMPONENT_CAMERA))
			continue;

		if (m_playerList.size() > 0) {
			// Find bounding rect around players in xz plane
			const vec3& playerPos = m_playerList[0]->transform.position;
			vec3 topLeft = playerPos;
			vec3 bottomRight = topLeft;
			for (auto player : m_playerList) {
				const vec3& playerPos = player->transform.position;
				topLeft = glm::min(topLeft, playerPos);
				bottomRight = glm::max(bottomRight, playerPos);
				bottomRight.y = topLeft.y; // Ensure all vehicles are projected down to a flat plane
			}
			
			vec3 centerOfAllPlayers = (topLeft + bottomRight) / 2.0f;
			vec3 cameraPos = centerOfAllPlayers + vec3{ 0, 100, 10 };
			vec3 dirCameraToTopLeft = normalize(topLeft - cameraPos);
			vec3 dirCameraToBottomRight = normalize(bottomRight - cameraPos);
			vec3 cameraForward = (dirCameraToTopLeft + dirCameraToBottomRight) / 2.0f;
			vec3 cameraLookAt = cameraPos + cameraForward;

			cameraEntity.camera.setLookAt(cameraPos, cameraLookAt, vec3{ 0, 1, 0 });
		}
	}
}

void CameraSystem::beginFrame()
{
}

void CameraSystem::endFrame()
{
}
