#include "TrackSystem.h"

#include "RenderSystem.h"
#include "Scene.h"

using namespace glm;

TrackSystem::TrackSystem(Scene& scene)
	: System(scene)
{
}

void TrackSystem::update()
{
	for (size_t i = 0; i < m_trackList.size() - 1; ++i) {
		//RenderSystem::drawDebugArrow(m_trackList[i] + vec3{ 0, 5, 0 }, m_trackList[i + 1] + vec3{ 0, 5, 0 });
	}
}

void TrackSystem::beginFrame()
{
}

void TrackSystem::endFrame()
{
}

void TrackSystem::initializeTrackSystem()
{
	for (size_t i = 0; i < m_scene.getEntityCount(); ++i) {
		Entity& entity = m_scene.getEntity(i);

		if (entity.hasComponents(COMPONENT_LEVELPROG)) {
			m_trackList.push_back(entity.transform.position);
		}
	}
}
