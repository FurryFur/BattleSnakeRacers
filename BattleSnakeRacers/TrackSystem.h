#pragma once

#include "System.h"

#include <glm\glm.hpp>

class Scene;

class TrackSystem : public System {
public:
	TrackSystem(Scene& scene);

	// Inherited via System
	virtual void update() override;
	virtual void beginFrame() override;
	virtual void endFrame() override;

	// Finds all track pieces and links them together.
	// Make sure all track pieces are in the scene before this is run.
	void initializeTrackSystem();

private:
	std::vector<glm::vec3> m_trackList;
	std::vector<std::vector<bool>> m_trackGrid;
	float m_gridSpacing;
};