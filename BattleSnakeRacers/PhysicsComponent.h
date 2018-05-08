#pragma once

#include <glm\glm.hpp>

struct PhysicsComponent {
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 angularVelocity;
};