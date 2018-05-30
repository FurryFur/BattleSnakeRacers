#include "CameraSystem.h"

#include "Entity.h"
#include "RenderSystem.h"
#include "Clock.h"
#include "GlobalConstants.h"

#include <glm\glm.hpp>
#include <glm\gtx\compatibility.hpp>

using namespace glm;

CameraSystem::CameraSystem(Scene& scene, std::vector<Entity*>& playerList)
	: System(scene)
	, m_playerList{ playerList }
	, m_isFirstUpdate{ true }
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
			bool foundNonDeadPlayer = false;
			vec3 topLeft, topRight, bottomLeft, bottomRight;
			for (auto player : m_playerList) {
				if (player->playerStats.getDeathState() == false) {
					const vec3& playerPos = player->transform.position;
					if (!foundNonDeadPlayer) {
						topLeft = topRight = bottomLeft = bottomRight = playerPos;
						foundNonDeadPlayer = true;
					}

					topLeft = glm::min(topLeft, playerPos);
					bottomRight = glm::max(bottomRight, playerPos);
					bottomRight.y = 0; // Ensure all vehicles are projected down to the XZ plane
					topRight = vec3{ bottomRight.x, 0, topLeft.z };
					bottomLeft = vec3{ topLeft.x, 0, bottomRight.z };
				}
			}

			// Don't do anything if everyone is dead.
			if (!foundNonDeadPlayer)
				return;

			vec3 centerOfAllPlayers = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;
			vec3 cameraPos = centerOfAllPlayers + Constants::cameraOffset;
			vec3 dirCameraToTopLeft = normalize(topLeft - cameraPos);
			vec3 dirCameraToTopRight = normalize(topRight - cameraPos);
			vec3 dirCameraToBottomLeft = normalize(bottomLeft - cameraPos);
			vec3 dirCameraToBottomRight = normalize(bottomRight - cameraPos);
			vec3 cameraForward = (dirCameraToTopLeft + dirCameraToTopRight + dirCameraToBottomLeft + dirCameraToBottomRight) / 4.0f;

			// Lerp camera position
			if (!m_isFirstUpdate) { // Prevents weird behavior on first frame, probably due to large delta time
				cameraPos = glm::lerp(cameraEntity.camera.getPosition(), cameraPos, Clock::getDeltaTime() * 20.0f);
				cameraForward = glm::lerp(cameraEntity.camera.getForward(), cameraForward, Clock::getDeltaTime() * 20.0f);
			}

			vec3 cameraLookAt = cameraPos + cameraForward;

			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToTopLeft * 2.0f, dirCameraToTopLeft, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToTopRight * 2.0f, dirCameraToTopRight, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToBottomLeft * 2.0f, dirCameraToBottomLeft, 50);
			//RenderSystem::drawDebugArrow(cameraPos + dirCameraToBottomRight * 2.0f, dirCameraToBottomRight, 50);
			//RenderSystem::drawDebugArrow(cameraPos + cameraForward * 2.0f, cameraForward, 50);

			cameraEntity.camera.setLookAt(cameraPos, cameraLookAt, vec3{ 0, 1, 0 });

			m_isFirstUpdate = false;
		}
	}
}

void CameraSystem::beginFrame()
{
}

void CameraSystem::endFrame()
{
}
