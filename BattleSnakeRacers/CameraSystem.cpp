#include "CameraSystem.h"

#include "Entity.h"
#include "RenderSystem.h"

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
			vec3 topRight = topLeft;
			vec3 bottomLeft = topLeft;
			for (auto player : m_playerList) {
				const vec3& playerPos = player->transform.position;
				topLeft = glm::min(topLeft, playerPos);
				bottomRight = glm::max(bottomRight, playerPos);
				bottomRight.y = 0; // Ensure all vehicles are projected down to the XZ plane
				topRight = vec3{ bottomRight.x, 0, topLeft.z };
				bottomLeft = vec3{ topLeft.x, 0, bottomRight.z };
			}

			vec3 centerOfAllPlayers = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;
			vec3 cameraPos = centerOfAllPlayers + vec3{ 0, 100, 10 };
			vec3 dirCameraToTopLeft = normalize(topLeft - cameraPos);
			vec3 dirCameraToTopRight = normalize(topRight - cameraPos);
			vec3 dirCameraToBottomLeft = normalize(bottomLeft - cameraPos);
			vec3 dirCameraToBottomRight = normalize(bottomRight - cameraPos);
			vec3 cameraForward = (dirCameraToTopLeft + dirCameraToTopRight + dirCameraToBottomLeft + dirCameraToBottomRight) / 4.0f;
			vec3 cameraLookAt = cameraPos + cameraForward;

			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToTopLeft * 2.0f, dirCameraToTopLeft, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToTopRight * 2.0f, dirCameraToTopRight, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToBottomLeft * 2.0f, dirCameraToBottomLeft, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToBottomRight * 2.0f, dirCameraToBottomRight, 50);
			//RenderSystem::drawDebugArrow(cameraPos + cameraForward * 2.0f, cameraForward, 50);

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
