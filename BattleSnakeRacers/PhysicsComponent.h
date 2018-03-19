#pragma once

#include <glm\glm.hpp>

struct PhysicsComponent;

struct PhysicsComponent {
	glm::vec3 velocity;
	glm::vec3 acceleration;
};