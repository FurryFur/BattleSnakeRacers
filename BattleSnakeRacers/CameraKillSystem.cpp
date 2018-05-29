#include "CameraKillSystem.h"

#include "GlobalConstants.h"
#include "Entity.h"
#include "Log.h"
#include "Game.h"
#include "Clock.h"

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

void CameraKillSystem::disableFor(float seconds)
{
	disabledRemainingDuration = seconds;
}

void CameraKillSystem::update()
{
	// Count down the disabled timer if one is set
	if (disabledRemainingDuration > 0) {
		disabledRemainingDuration -= Clock::getDeltaTime();
		return;
	}

	// Do nothing if no camera has been set
	if (!m_killCamera)
		return;

	// Find the player in last place, this player is a candidate to be killed
	bool isFrstNonDeadPlayer = true;
	vec3 topLeft, topRight, bottomLeft, bottomRight;
	Entity* lastPlayer = nullptr;
	for (Entity* player : m_playerList) {
		if (player->playerStats.getDeathState() == true)
			continue;

		// Update last player found so far
		if (!lastPlayer)
			lastPlayer = player;
		else if (player->playerStats.lap < lastPlayer->playerStats.lap 
			  || (player->playerStats.lap == lastPlayer->playerStats.lap 
			   && player->playerStats.progression < lastPlayer->playerStats.progression)) {
			lastPlayer = player;
		}
	}

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
