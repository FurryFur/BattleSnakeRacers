#include "CameraKillSystem.h"

#include "GlobalConstants.h"
#include "Entity.h"
#include "Log.h"
#include "Game.h"

#include <glm\glm.hpp>

using namespace glm;

CameraKillSystem::CameraKillSystem(Scene& scene, const std::vector<Entity*>& playerList, Entity* killCamera)
	: System(scene)
	, m_playerList{ playerList }
	, m_killCamera{ killCamera }
{
}

void CameraKillSystem::setKillCamera(Entity* killCamera)
{
	m_killCamera = killCamera;
}

void CameraKillSystem::update()
{
	if (!m_killCamera)
		return;

	// Find the player in last place, this player is a candidate to be killed
	bool isFrstNonDeadPlayer = true;
	vec3 topLeft, topRight, bottomLeft, bottomRight;
	Entity* lastPlayer = nullptr;
	for (Entity* player : m_playerList) {
		if (player->playerStats.getDeathState() == true)
			continue;

		// Find bounding rect around players in xz plane
		const vec3& playerPos = player->transform.position;
		if (isFrstNonDeadPlayer) {
			topLeft = topRight = bottomLeft = bottomRight = playerPos;
			isFrstNonDeadPlayer = false;
		}
		topLeft = glm::min(topLeft, playerPos);
		bottomRight = glm::max(bottomRight, playerPos);
		bottomRight.y = 0; // Ensure all vehicles are projected down to the XZ plane
		topRight = vec3{ bottomRight.x, 0, topLeft.z };

		// Update last player found so far
		if (!lastPlayer)
			lastPlayer = player;
		else if (player->playerStats.progression < lastPlayer->playerStats.progression) {
			lastPlayer = player;
		}
	}

	// Disable camera kill if too far away from player center
	vec3 centerOfAllPlayers = (topLeft + topRight + bottomLeft + bottomRight) / 4.0f;
	vec3 targetCameraPos = centerOfAllPlayers + Constants::cameraOffset;
	vec3 displacementToTarget = targetCameraPos - m_killCamera->camera.getPosition();
	if (length(displacementToTarget) > 5.0f)
		return;

	if (lastPlayer) {
		// Get last players position in normalized device coordinate space
		vec4 pos = vec4(lastPlayer->transform.position, 1);
		const mat4& view = m_killCamera->camera.getView();
		const mat4& proj = m_killCamera->camera.getProjection();
		vec4 ndcPos = proj * view * pos;
		ndcPos /= ndcPos.w;

		// Check if player is off screen
		if (ndcPos.x < -1 || ndcPos.x > 1 || ndcPos.y < -1 || ndcPos.y > 1) {
			Game::killPlayer(*lastPlayer);
		}
	}
}

void CameraKillSystem::beginFrame()
{
}

void CameraKillSystem::endFrame()
{
}
