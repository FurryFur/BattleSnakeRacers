#pragma once

#include <glm\glm.hpp>

#include <array>

struct ContactInfo {
	bool isCollision;
	std::array<glm::vec3, 2> contactNormals;
	float penetrationDistance;
};

struct SphereCollisionComponent {
	float radius;
};

struct BoxCollisionComponent {
	glm::vec3 extents;
};

struct CapsuleCollisionComponent {
	glm::vec3 base;
	glm::vec3 head;
	float radius;
};