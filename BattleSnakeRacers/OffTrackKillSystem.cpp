#include "OffTrackKillSystem.h"

#include "GlobalConstants.h"
#include "Game.h"
#include "Audio.h"

#include <glm\glm.hpp>

#include <limits>

using namespace glm;

OffTrackKillSystem::OffTrackKillSystem(Scene& scene, const std::vector<Entity*>& playerList, float killY)
	: System(scene)
	, m_playerList{ playerList }
	, m_killY{ killY }
{
}

void OffTrackKillSystem::update()
{
	// Loop through alive players
	for (size_t i = 0; i < m_playerList.size(); ++i) {
		Entity& player = *m_playerList[i];
		if (player.playerStats.getDeathState() == false) {

			// Find players nearest track tile
			vec2 offsetFromNearestTile = vec2{ std::numeric_limits<decltype(vec3::x)>::max(), std::numeric_limits<decltype(vec3::z)>::max() };
			for (size_t j = 0; j < m_scene.getEntityCount(); ++j) {
				Entity& entity = m_scene.getEntity(j);

				if (entity.hasComponents(COMPONENT_LEVELPROG)) {
					vec3 playerPos = player.transform.position;
					vec2 xzPlayerPos = { playerPos.x, playerPos.z };
					vec3 tilePos = entity.transform.position;
					vec2 xzTilePos = { tilePos.x, tilePos.z };
					vec2 offset = xzPlayerPos - xzTilePos;
					if (length(offset) < length(offsetFromNearestTile))
						offsetFromNearestTile = offset;
				}
			}

			// If outside of tile bounds
			if (offsetFromNearestTile.x >  Constants::trackTileExtents.x || offsetFromNearestTile.y >  Constants::trackTileExtents.y
			 || offsetFromNearestTile.x < -Constants::trackTileExtents.x || offsetFromNearestTile.y < -Constants::trackTileExtents.y) {
				// then make the player fall.
				player.physics.acceleration += vec3{ 0, -50, 0 };
				player.transform.eulerAngles.y += 1.0f;
				player.removeComponents(COMPONENT_INPUT);
			}

			// If below a certain Y value and on the track again
			else if (player.transform.position.y <= -4.0f)
			{
				Game::killPlayer(*m_playerList[i]);
				// Play a sound
				Audio& audio = Audio::getInstance();
				audio.playSFX(PLAYER_SNAKE_COLLIDE);
			}
			else
			{
				player.physics.velocity.y = 0.0f;
			}

			// Kill players beyond a certain -y value
			if (player.transform.position.y <= m_killY) {
				Game::killPlayer(*m_playerList[i]);
				// Play a sound
				Audio& audio = Audio::getInstance();
				audio.playSFX(PLAYER_SNAKE_COLLIDE);
			}
		}
	}
}

void OffTrackKillSystem::beginFrame()
{
}

void OffTrackKillSystem::endFrame()
{
}
